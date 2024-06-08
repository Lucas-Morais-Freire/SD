#define NUM_BUTTONS 7
#define BUTTON_DELAY 5 // (ms)

// Estados dos botões e se foram logados
bool button_states[NUM_BUTTONS] = {false};
bool button_logged[NUM_BUTTONS] = {false};

// Buffer para armazenar entradas
char inputs[100];
int input_length = 0;

// Funções de ação dos botões
void button_press(int button);
void button_hold(int button);

// Configura a comunicação serial (USART)
void setup_serial() {
    UCSR0A |= (1 << U2X0);
    UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);  
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 
    UCSR0C &= ~(1 << UCPOL0);              
    UBRR0 = 0;                               

    sei(); 
}

void send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void send_message_with_length() {
    // Envia o comprimento da mensagem (input_length) como um número
    send_char(input_length);

    // Envia os caracteres da mensagem
    for (int i = 0; i < input_length; i++) {
        send_char(inputs[i]);
    }
}

void log_button_press(int button) {
    char c;
    switch (button) {
        case 0: c = 'a'; break;
        case 1: c = 'b'; break;
        case 2: c = 'r'; break;
        case 3: c = 'l'; break;
        case 4: c = 'd'; break;
        case 5: c = 'u'; break;
        case 6: c = 's'; break;
        default: return;
    }
    button_logged[button] = true;
    inputs[input_length++] = c;
    button_press(button);
}

void log_button_hold(int button) {
    // Apenas adiciona o caractere de hold se o botão ainda não foi logado
    if (!button_logged[button]) {
        char c;
        switch (button) {
            case 2: c = 'R'; break;
            case 3: c = 'L'; break;
            case 4: c = 'D'; break;
            case 5: c = 'U'; break;
            default: return;
        }
        inputs[input_length++] = c;
        button_logged[button] = true;
        button_hold(button);
    }
}

void button_press(int button) {
}

void button_hold(int button) {
}

// Interrupção 
ISR(USART_RX_vect) {
    char incomingChar = UDR0;

    memset(button_logged, false, sizeof(button_logged));
    switch (incomingChar) {
        case 'a':
            send_char('k');
            break;
        case 's':
            send_message_with_length();
            input_length = 0; // Limpar entradas após enviar
            break;
    }
}

int main() {
    // Configura botões como entrada com pull-up
    DDRB &= 0b11000000; // PB0 a PB5 como entrada
    PORTB |= 0b00111111; // PB0 a PB5 com pull-up
    DDRC &= ~(1 << PC0); // PC0 como entrada
    PORTC |= (1 << PC0); // PC0 com pull-up

    setup_serial();

    while (true) {
        // Processa entradas dos botões
        for (int i = 0; i < NUM_BUTTONS; i++) {
            bool isPressed = (i < 6) ? !(PINB & (1 << i)) : !(PINC & (1 << PC0));
            if (!button_states[i]) {
                if (isPressed) {
                    log_button_press(i);
                    button_states[i] = true;
                }
            } else {
                if (!isPressed) {
                    button_states[i] = false;
                } else {
                    log_button_hold(i);
                }
            }
        }

        // Adiciona um pequeno delay para debounce
        _delay_ms(BUTTON_DELAY);
    }

    return 0;
}
