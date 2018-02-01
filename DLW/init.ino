void setupLEDs() {
  strip.begin();
  strip.show();
}

void setupLCDdisplay() {
  lcd.begin(16, 2);
  lcd.print("*DLW MEGA  V4.0*");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void setupSDcard() {
  pinMode(SDSSPIN, OUTPUT);

  while (!SD.begin(SDSSPIN)) {
    BackLightOn();
    lcd.print("SD init failed! ");
    delay(1000);
    lcd.clear();
    delay(500);
  }
  lcd.clear();
  lcd.print("SD init done.   ");
  delay(500);
  root = SD.open("/");
  lcd.clear();
  lcd.print("Scanning files  ");
  delay(500);
  GetFileNamesFromSD(root);
  isort(m_FileNames, m_NumberOfFiles);
  m_CurrentFilename = m_FileNames[0];
  DisplayCurrentFilename();
}
