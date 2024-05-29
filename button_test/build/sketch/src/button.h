#line 1 "C:\\Projetos\\github\\SD\\button_test\\src\\button.h"
#ifndef BUTTON_H
#define BUTTON_H
#include <avr/io.h>

namespace ntd {
class button {
private:
	bool state;             // estado do botao (apertado, desapertado)
	void (*func_press)();   // funcao para executar ao apertar
	void (*func_hold)();    // funcao para executar ao segurar
	volatile uint8_t* PINx; // o port de leitura
	uint8_t Pxn;            // id do pino
	uint16_t time;          // timestamp da troca de estado
	uint64_t cycle;         // o ciclo correspondente a troca de estado
	uint64_t delay;         // a quantidade de delay do debounce delay*timer1_prescaler/f_osc segundos
public:
	/**
	 * @brief Construct a new Button object
	 * 
	 * @param func_press Funcao ativada ao pressionar o botao
	 * @param func_hold  Funcao chamada enquanto o botao eh pressionado
	 * @param PINx       Porta de leitura do pino
	 * @param Pxn 		 ID do pino (deve ter sido previamente configurado como INPUT_PULLUP)
	 * @param delay      Quantidade de delay em microssegundos
	 */
	button(void (*func_press)(), void (*func_hold)(), volatile uint8_t* PINx, uint8_t Pxn, uint64_t delay);
	/**
	 * @brief Constroi um objeto de botao padrao
	 * 
	 */
	button() : button(nullptr, nullptr, nullptr, 0, 0) {};
	/**
	 * @brief Checa o estado atual do botao e realiza as operacoes necessarias
	 * 
	 */
	void check_button();
};
}

#endif