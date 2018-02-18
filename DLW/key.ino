int ReadKeypad() {
  adc_key_in = analogRead(0);             // read the value from the sensor
  aux_btn_in = digitalRead(AuxButton);
  key = get_key(adc_key_in, aux_btn_in);  // convert into key press
  Serial.println(key);
  if (key != oldkey) {                    // if keypress is detected
    delay(50);                            // wait for debounce time
    adc_key_in = analogRead(0);           // read the value from the sensor
    aux_btn_in = digitalRead(AuxButton);
    key = get_key(adc_key_in, aux_btn_in);// convert into key press
    if (key != oldkey) {
      oldkey = key;
      return key;
    }
  }
  else if (key != keys::SELECT) return key; // allow fast scrolling exept for select
  else return -1;
}

// Convert ADC value to key number
int get_key(int input, bool aux_btn_in) {
  
  if (!aux_btn_in) return keys::SELECT;  // Return if auxbtn is pressed
  int k;
  for (k = 0; k < NUM_KEYS; k++) {
    if (input < adc_key_val[k]) {
      return k;
    }
  }
  if (k >= NUM_KEYS)
    k = -1;                               // No valid key pressed
  return k;
}
