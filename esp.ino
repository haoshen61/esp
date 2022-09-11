void setup() {
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
}

void loop() {
  analogWrite(23, 255);
  analogWrite(22, 255);
  analogWrite(21, 255);
}