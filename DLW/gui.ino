void updateMenu(){
  switch (menuItem) {
      case 1:
        lcd.begin(16, 2);
        lcd.print("1:File Select   ");
        lcd.setCursor(0, 1);
        lcd.print(m_CurrentFilename);
        break;
      case 2:
        lcd.begin(16, 2);
        lcd.print("2:Brightness    ");
        lcd.setCursor(0, 1);
        lcd.print(brightness);
        if (brightness == 100) {
          lcd.setCursor(3, 1);
        }
        else {
          lcd.setCursor(2, 1);
        }
        lcd.print("%");
        break;
      case 3:
        lcd.begin(16, 2);
        lcd.print("3:Init Delay    ");
        lcd.setCursor(0, 1);
        lcd.print(initDelay);
        break;
      case 4:
        lcd.begin(16, 2);
        lcd.print("4:Frame Delay   ");
        lcd.setCursor(0, 1);
        lcd.print(frameDelay);
        break;
      case 5:
        lcd.begin(16, 2);
        lcd.print("5:Repeat Times  ");
        lcd.setCursor(0, 1);
        if (repeatTimes > 0) lcd.print(repeatTimes);
        else lcd.print("infinite");
        break;
      case 6:
        lcd.begin(16, 2);
        lcd.print("6:Repeat Delay  ");
        lcd.setCursor(0, 1);
        lcd.print(repeatDelay);
        break;
    }
}

void handleKeys(int keypress){
  if (keypress == keys::SELECT && menuItem != 7) {    // The select key was pressed
    analogWrite(BACKLIGHT, 0);                        // Turn off Backlight
    digitalWrite(IndicatorLed, LOW);                  // Turn off Indicator LED
    delay(initDelay);
    if (repeatTimes > 1) {
      for (int x = repeatTimes; x > 0; x--) {
        SendFile(m_CurrentFilename);
        delay(repeatDelay);
      }
    }
    else if (repeatTimes == 0) {
      int keypress;
      do{
        SendFile(m_CurrentFilename);
        keypress = ReadKeypad();
      }while(keypress == -1);
    }
    else SendFile(m_CurrentFilename);
    ClearStrip(0);
    BackLightOn();
    digitalWrite(IndicatorLed, HIGH);
  }
  
  }

  else if (keypress == keys::RIGHT) {           // The Right Key was Pressed
    switch (menuItem) {
      case 1:                             // Select the Next File
        if (m_FileIndex < m_NumberOfFiles - 1) {
          m_FileIndex++;
        }
        else m_FileIndex = 0;             // On the last file so wrap round to the first file
        DisplayCurrentFilename();
        break;
      case 2:                             // Adjust Brightness
        if (brightness < 100) brightness++;
        break;
      case 3:                             // Adjust Initial Delay + 1 second
        initDelay += 1000;
        break;
      case 4:                             // Adjust Frame Delay + 1 milliseconds
        frameDelay++;
        break;
      case 5:                             // Adjust Repeat Times + 1
        repeatTimes++;
        break;
      case 6:                             // Adjust Repeat Delay + 100 milliseconds
        repeatDelay += 100;
        break;
    }
  }

  else if (keypress == keys::LEFT) {            // The Left Key was Pressed
    switch (menuItem) {                   // Select the Previous File
      case 1:
        if (m_FileIndex > 0) m_FileIndex--;
        else m_FileIndex = m_NumberOfFiles - 1; // On the last file so wrap round to the first file
        DisplayCurrentFilename();
        delay(500);
        break;
      case 2:                                   // Adjust Brightness
        if (brightness > 1) brightness--;
        break;
      case 3:                                   // Adjust Initial Delay - 1 second
        if (initDelay > 0) initDelay -= 1000;
        break;
      case 4:                                   // Adjust Frame Delay - 1 millisecond
        if (frameDelay > 0) frameDelay--;
        break;
      case 5:                                   // Adjust Repeat Times - 1
        if (repeatTimes > 0)  repeatTimes--;
        break;
      case 6:                                   // Adjust Repeat Delay - 100 milliseconds
        if (repeatDelay > 0) repeatDelay -= 100;
        break;
    }
  }

  else if (( keypress == keys::UP)) {                 // The up key was pressed
    if (menuItem == 1) menuItem = 6;
    else menuItem--;
  }
  
  else if (( keypress == keys::DOWN)) {                 // The down key was pressed
    if (menuItem == 6) menuItem = 1;
    else  menuItem += 1;
  }
}

