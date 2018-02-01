void SendFile(String Filename) {
  char temp[14];
  Filename.toCharArray(temp, 14);
  dataFile = SD.open(temp);

  // if the file is available send it to the LED's
  if (dataFile) {
    ReadTheFile();
    dataFile.close();
  }
  else {
    lcd.clear();
    lcd.print("  Error reading ");
    lcd.setCursor(4, 1);
    lcd.print("file");
    BackLightOn();
    delay(1000);
    lcd.clear();
    setupSDcard();
    return;
  }
}

void DisplayCurrentFilename() {
  m_CurrentFilename = m_FileNames[m_FileIndex];
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(m_CurrentFilename);
}

void GetFileNamesFromSD(File dir) {
  int fileCount = 0;
  String CurrentFilename = "";
  while (1) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      m_NumberOfFiles = fileCount;
      entry.close();
      break;
    }
    else {
      if (entry.isDirectory()) {
        //GetNextFileName(root);
      }
      else {
        CurrentFilename = entry.name();
        if (CurrentFilename.endsWith(".bmp") || CurrentFilename.endsWith(".BMP") ) { //find files with our extension only
          m_FileNames[fileCount] = entry.name();
          fileCount++;
        }
      }
    }
    entry.close();
  }
}

void latchanddelay(int dur) {
  strip.show();
  delay(dur);
}

void ClearStrip(int duration) {
  int x;
  for (x = 0; x < STRIP_LENGTH; x++) {
    strip.setPixelColor(x, 0);
  }
  strip.show();
}

uint32_t readLong() {
  uint32_t retValue;
  byte incomingbyte;

  incomingbyte = readByte();
  retValue = (uint32_t)((byte)incomingbyte);

  incomingbyte = readByte();
  retValue += (uint32_t)((byte)incomingbyte) << 8;

  incomingbyte = readByte();
  retValue += (uint32_t)((byte)incomingbyte) << 16;

  incomingbyte = readByte();
  retValue += (uint32_t)((byte)incomingbyte) << 24;

  return retValue;
}

uint16_t readInt() {
  byte incomingbyte;
  uint16_t retValue;

  incomingbyte = readByte();
  retValue += (uint16_t)((byte)incomingbyte);

  incomingbyte = readByte();
  retValue += (uint16_t)((byte)incomingbyte) << 8;

  return retValue;
}

int readByte() {
  int retbyte = -1;
  while (retbyte < 0) retbyte = dataFile.read();
  return retbyte;
}

void getRGBwithGamma() {
  g = gamma(readByte()) / (101 - brightness);
  b = gamma(readByte()) / (101 - brightness);
  r = gamma(readByte()) / (101 - brightness);
}

void ReadTheFile() {
#define MYBMP_BF_TYPE           0x4D42
#define MYBMP_BF_OFF_BITS       54
#define MYBMP_BI_SIZE           40
#define MYBMP_BI_RGB            0L
#define MYBMP_BI_RLE8           1L
#define MYBMP_BI_RLE4           2L
#define MYBMP_BI_BITFIELDS      3L

  uint16_t bmpType = readInt();
  uint32_t bmpSize = readLong();
  uint16_t bmpReserved1 = readInt();
  uint16_t bmpReserved2 = readInt();
  uint32_t bmpOffBits = readLong();
  bmpOffBits = 54;

  /* Check file header */
  if (bmpType != MYBMP_BF_TYPE || bmpOffBits != MYBMP_BF_OFF_BITS) {
    lcd.setCursor(0, 0);
    lcd.print("not a bitmap");
    delay(1000);
    return;
  }

  /* Read info header */
  uint32_t imgSize = readLong();
  uint32_t imgWidth = readLong();
  uint32_t imgHeight = readLong();
  uint16_t imgPlanes = readInt();
  uint16_t imgBitCount = readInt();
  uint32_t imgCompression = readLong();
  uint32_t imgSizeImage = readLong();
  uint32_t imgXPelsPerMeter = readLong();
  uint32_t imgYPelsPerMeter = readLong();
  uint32_t imgClrUsed = readLong();
  uint32_t imgClrImportant = readLong();

  /* Check info header */
  if ( imgSize != MYBMP_BI_SIZE || imgWidth <= 0 ||
       imgHeight <= 0 || imgPlanes != 1 ||
       imgBitCount != 24 || imgCompression != MYBMP_BI_RGB ||
       imgSizeImage == 0 )
  {
    lcd.setCursor(0, 0);
    lcd.print("Unsupported");
    lcd.setCursor(0, 1);
    lcd.print("Bitmap Use 24bpp");
    delay(1000);
    return;
  }
  
  int displayWidth = imgWidth;
  if (imgWidth > STRIP_LENGTH) {
    displayWidth = STRIP_LENGTH;           //only display the number of led's we have
  }
  
  /* compute the line length */
  uint32_t lineLength = imgWidth * 3;
  if ((lineLength % 4) != 0)
    lineLength = (lineLength / 4 + 1) * 4;
    
  // Note:
  // The x,r,b,g sequence below might need to be changed if your strip is displaying
  // incorrect colors.  Some strips use an x,r,b,g sequence and some use x,r,g,b
  // Change the order if needed to make the colors correct.

  for (int y = imgHeight; y > 0; y--) {
    int bufpos = 0;
    for (int x = 0; x < displayWidth; x++) {
      uint32_t offset = (MYBMP_BF_OFF_BITS + (((y - 1) * lineLength) + (x * 3))) ;
      dataFile.seek(offset);

      getRGBwithGamma();

      strip.setPixelColor(x, r, b, g);

    }
    latchanddelay(frameDelay);
  }
}

// Sort the filenames in alphabetical order
void isort(String *filenames, int n) {
  for (int i = 1; i < n; ++i) {
    String j = filenames[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < filenames[k]); k--) {
      filenames[k + 1] = filenames[k];
    }
    filenames[k + 1] = j;
  }
}

PROGMEM const unsigned char gammaTable[]  = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,
  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  7,  7,
  7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11,
  11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16,
  16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
  23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30,
  30, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39,
  40, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50,
  50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62,
  62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 74, 74, 75,
  76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
  92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 104, 105, 106, 107, 108,
  109, 110, 111, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 125, 126, 127
};

inline byte gamma(byte x) {
  return pgm_read_byte(&gammaTable[x]);
}
