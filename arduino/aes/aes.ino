#include "aes.h"
#include "key.h"

unsigned char  key[176];

void setup() {
  Serial.begin(9600);
  loadKey(key,30);
}

void loop() {
  //unsigned char  key[176] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  //unsigned char  wrd[16] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  unsigned char  wrd[16];
  unsigned char  tmp[16];
  //++++++++++++++++++
  if (Serial.available() > 0) {
      byte keyk = Serial.read();
      if(keyk == 'e'){
        byte cn = 0;
        while (cn < 16){
          if (!Serial.available())
            continue;
          wrd[cn] = Serial.read();
          cn ++;           
        }
        AES_encrypt_128(wrd, key, tmp);
        for(byte i = 0; i < 16; i++)
          Serial.write(wrd[i]);
      }else if(keyk == 'd'){
        byte cn = 0;
        while (cn < 16){
          if (!Serial.available())
            continue;
          wrd[cn] = Serial.read();
          cn ++;           
        }
        AES_decrypt_128(wrd, key, tmp);
        for(byte i = 0; i < 16; i++)
          Serial.write(wrd[i]);
      }else if(keyk == 'k'){
        byte cn = 0;
        while (cn < 16){
          if (!Serial.available())
            continue;
          key[cn] = Serial.read();
          cn ++;           
        }
        saveKey(key,30);    
      }
  }
  /*
  Serial.print("\n");
  for(unsigned char i=0;i<16;i++)
    Serial.print(wrd[i]);
  Serial.print("\n\n");
  AES_encrypt_128(wrd, key, tmp);
  Serial.print("\n");
  for(unsigned char i=0;i<16;i++)
    Serial.print(wrd[i]);
  Serial.print("\n\n");
  Serial.print("\n\n");
  AES_decrypt_128(wrd, key, tmp);
  Serial.print("\n");
  for(unsigned char i=0;i<16;i++)
    Serial.print(wrd[i]);
  Serial.print("\n\n");
  delay(1000000);
  */
}
