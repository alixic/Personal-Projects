void setup() {
  // put your setup code here, to run once:

   pinMode(A0, INPUT); // start - stop
   pinMode(A1, INPUT);
   pinMode(A2, INPUT);

  pinMode(0, INPUT); // - ir stanga sasiu
  pinMode(1, INPUT); // - ir stanga frontal
  pinMode(2, INPUT); // - ir fata frontal
  pinMode(3, INPUT); // - ir dreapta sasiu
  pinMode(4, INPUT); // - ir frontal dreapta


  pinMode(5, INPUT); // dip switch 1 - pt testare - pornire motoare
  pinMode(6, INPUT);
  pinMode(7, INPUT);

 // pinMode(10, OUTPUT); // - control viteza - motor stanga - PWM
 // pinMode(11, OUTPUT); // - control viteza - motor dreapta - PWM

  //pinMode(12, OUTPUT); // - sens rotatie - motor stanga
 // pinMode(13, OUTPUT); // - sens rotatie - motor dreapta
  Serial.begin(9600);

  Serial.print(digitalRead(0));
  Serial.print(" ");
  Serial.print(digitalRead(1));
  Serial.print(" ");
  Serial.print(digitalRead(2));
  Serial.print(" ");
  Serial.print(digitalRead(3));
  Serial.print(" ");
  Serial.print(digitalRead(4));
  Serial.print(" ");
  Serial.println("");


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(0));
  Serial.print(" ");
  Serial.print(digitalRead(1));
  Serial.print(" ");
  Serial.print(digitalRead(2));
  Serial.print(" ");
  Serial.print(digitalRead(3));
  Serial.print(" ");
  Serial.print(digitalRead(4));
  Serial.print(" ");
  Serial.print(analogRead(A4));
  Serial.print(" ");
  Serial.print(analogRead(A5));
  Serial.print(" ");
  Serial.println("");

  delay(100);
}
