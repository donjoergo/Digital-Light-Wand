void LightSaberOn(){
  getSaberColor();
  strip.setBrightness(120);
  for (int n = 0; n < STRIP_LENGTH; n+=3){
    strip.setPixelColor(n, r, g, b);
    strip.setPixelColor(n+1, r, g, b);
    strip.setPixelColor(n+2, r, g, b); 
    strip.show();
  }
}

void LightSaberOff(){
  for (int n = STRIP_LENGTH; n > 0; n-=3){
    strip.setPixelColor(n, 0, 0, 0);
    strip.setPixelColor(n-1, 0, 0, 0);
    strip.setPixelColor(n-2, 0, 0, 0);
    strip.show();
  }
  delayMicroseconds(100);
}

void getSaberColor(){
  switch (saberColorIndicator) {
    case 0:
      r=255; g=0; b=0;
      break;
    case 1:
      r=0; g=255; b=0;
      break;
    case 2:
      r=0; g=0; b=255;
      break;
    default:
      r=255; g=255; b=255;
      break;
  }
}
