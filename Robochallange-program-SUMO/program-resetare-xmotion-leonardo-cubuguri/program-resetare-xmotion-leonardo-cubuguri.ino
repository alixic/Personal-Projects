void setup() {
  // put your setup code here, to run once:
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(600);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  delay(600);

}


// apas pe reset apoi dam upload repede si cand scrie uploading... mai apasam odata reset