// C++ code
//
char s = 0;

void setup()
{
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(42, OUTPUT);
  digitalWrite(40,LOW);
  digitalWrite(41,LOW);
  digitalWrite(42,LOW);
}

void loop()
{
  if (s == 0) {
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, LOW);
    s = 1;
  } else if (s == 1) {
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, LOW);
    s = 2;
  } else {
    digitalWrite(40, LOW);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    s = 0;
  }
  delay(1000);
}
