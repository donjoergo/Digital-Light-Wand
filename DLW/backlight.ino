void BackLightOn() {
  analogWrite(BACKLIGHT, LCD_BRIGHTNESS);
  BackLightTimer = true;
  BackLightTemp =  BackLightTimeout;
}

void BackLightTime() {
  if ((BackLightTemp <= 255) && (BackLightTemp >= LCD_BRIGHTNESS)) {
    BackLightTemp = LCD_BRIGHTNESS;
  }
  if ((BackLightTemp <= 255) && (BackLightTemp >= 0)) {
    analogWrite(BACKLIGHT, BackLightTemp);
  }

  if (BackLightTemp <= 0) {
    BackLightTimer = false;
    BackLightTemp =  BackLightTimeout;
    analogWrite(BACKLIGHT, 0);
  }
  else {
    BackLightTemp -= 15;
  }
}
