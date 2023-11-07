#include <DES.h>
DES des;
//byte data_in[30];
//String  input;
const byte MaxByteArraySize = 8;
char buf_pass_input[8];
void setup()
{
    Serial.write(0x0C);
    Serial.begin(230400);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("================================================================");
    Serial.println("|                                                              |");
    Serial.println("|           Welcome to Nigel SCA Encryption Demo App           |");
    Serial.println("|           Single DES encryption only                         |");
    Serial.println("|           key = 0123456789abcdef                             |");
    Serial.println("|                                                              |");
    Serial.println("================================================================");
    Serial.println("");
    Serial.println("Please enter your 8-byte plaintext data here:");

}

void DES_func(byte *des_data_in) 
{
  byte data_in[8];
  int charToInt;
  char buf_temp[8]; 
  byte data_out[8];
  byte key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

  //  ========================================================
  //  Encrypt
  Serial.println("\nDES encryption is done");
  des.encrypt(data_out, des_data_in, key);
  dumpByteArray(des_data_in, MaxByteArraySize);
  dumpByteArray(data_out, MaxByteArraySize);

  //  ========================================================
  //  Decrypt
  //  comment below
//  for (int i = 0; i < 8; i++)
//  {
//    data_in[i] = data_out[i];
//  }
//  Serial.print("Decrypt...");
//  Serial.println("The passed in Ciphertext:");
//  time = micros();
//  des.tripleDecrypt(data_out, data_in, key);
//  time = micros() - time;
//  Serial.print("done. (");
//  Serial.print(time);
//  Serial.println(" micros)");
//  dumpByteArray(data_out, MaxByteArraySize);
//  delay(100);
  //  comment above
}


void dumpByteArray(const byte * byteArray, const byte arraySize)
{
  for (int i = 0; i < arraySize; i++)
  {
    if (byteArray[i] < 0x10)
      Serial.print("0");
    Serial.print(byteArray[i], HEX);
  }
  Serial.println();
}


void hexCharacterStringToBytes(byte *byteArray, const char *hexString)
{
  bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    bool oddCharIndex = charIndex & 1;
    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}


byte nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return 0;  // Not a valid hexadecimal character
}


void loop() {

  while (Serial.available() > 0) 
  {
    String  entered_input = Serial.readStringUntil('\n'); // read the incoming data as string
    byte input_in_byte[MaxByteArraySize] = {0};
    const char* input_char = entered_input.c_str();
    hexCharacterStringToBytes(input_in_byte, input_char);
    digitalWrite(LED_BUILTIN, HIGH);
    DES_func(input_in_byte);     // pass the input data (in byte) to encryption funcs
    digitalWrite(LED_BUILTIN, LOW);
  }
}
