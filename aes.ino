#define ROTL8(x,shift) ((byte) ((x) << (shift)) | ((x) >> (8 - (shift)))) // wiki

//
void rotate(unsigned char *in) {
        unsigned char a,c;
        a = in[0];
        for(c=0;c<3;c++) 
                in[c] = in[c + 1];
        in[3] = a;
        return;
}

//
unsigned char rcon(unsigned char in) {
        unsigned char c=1;
        if(in == 0)  
                return 0; 
        while(in != 1) {
    unsigned char b;
    b = c & 0x80;
    c <<= 1;
    if(b == 0x80) {
      c ^= 0x1b;
    }
                in--;
        }
        return c;
}

// wiki
byte sbox(byte p_get) {
  /* loop invariant: p * q == 1 in the Galois field */
  byte p = 1, q = 1, tmp;
  do {
    /* multiply p by x+1 */
    p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
    /* divide q by x+1 */
    q ^= q << 1;
    q ^= q << 2;
    q ^= q << 4;
    q ^= q & 0x80 ? 0x09 : 0;
    /* compute the affine transformation */
    tmp = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    //sbox[p] = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    if(p_get == p){
        return tmp;
    }
  } while (p != 1);
  /* 0 is a special case since it has no inverse */
  return 0x63;
}

byte sbox_inv(byte p_get) {
  /* loop invariant: p * q == 1 in the Galois field */
  byte p = 1, q = 1, tmp;
  do {
    /* multiply p by x+1 */
    p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
    /* divide q by x+1 */
    q ^= q << 1;
    q ^= q << 2;
    q ^= q << 4;
    q ^= q & 0x80 ? 0x09 : 0;
    /* compute the affine transformation */
    tmp = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    //sbox[p] = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    //isbox[sbox[p]] = p;
    //isbox[tmp] = p;
    if(tmp == p_get){
        return p;
    }
  } while (p != 1);
  /* 0 is a special case since it has no inverse */
  return 0;
}

/*
void sbox_field(unsigned char * fild,unsigned char * inv_fild){  byte p = 1, q = 1;
  do {
    p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
    q ^= q << 1;
    q ^= q << 2;
    q ^= q << 4;
    q ^= q & 0x80 ? 0x09 : 0;
    fild[p] = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    //sbox[p] = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
    inv_fild[fild[p]] = p;
  } while (p != 1);
  fild[0] = 0x63;
  inv_fild[0] = 0x52;
}
*/

// My work! beranm14
void printMatrix(unsigned char * wrd, byte x = 4, byte y = 4){
  for(byte i=0; i<x; i++){
    for(byte j=0; j<y; j++){   
      Serial.print(((byte)wrd[(4*i)+j]),HEX);
      Serial.print(" "); 
    } 
    Serial.print("\n");
  }
}

void printExpKey(unsigned char * key){
  byte i = 0;
  while (i<176){
    if(i%16 == 0)
      Serial.print("\n");
    Serial.print(((byte)key[i]),HEX);
    i++;
  }
  Serial.print("\n");
}


void printSbox(){
  for(byte y = 0; y < 16 ;y++){
    for(byte x = 0; x < 16 ;x++){
      Serial.print( sbox( ((y << 4 | x)) ),HEX);
      Serial.print(" ");
      Serial.print((y << 4 | x));
      Serial.print("|");
    }
    Serial.print("\n");
  }
}
void printInvSbox(){
  for(byte y = 0; y < 16 ;y++){
    for(byte x = 0; x < 16 ;x++){
      //Serial.print(sbox_inv( (y << 4 | x) ),HEX);
      //Serial.print(" ");
      Serial.print( sbox_inv( (y << 4 | x) ),HEX);
      Serial.print(" ");
      Serial.print((y << 4 | x));
      Serial.print("|");
    }
    Serial.print("\n");
  }
}

// My work! beranm14
void ShiftRows(unsigned char * wrd,unsigned char * tmp){
  translate_matrix(wrd);
  for(byte i=0; i<4; i++){
    for(byte j=0; j<4; j++){
      tmp[(4*j)+i] = wrd[(4*j)+(j+i)%4];   
    } 
  }
 for(byte i=0; i<16; i++){
    wrd[i] = tmp[i]; 
 }
 translate_matrix(wrd);
}
void InvShiftRows(unsigned char * wrd,unsigned char * tmp){
 translate_matrix(wrd);
 for(byte i=0; i<4; i++){
    for(byte j=0; j<4; j++){
      tmp[(4*j)+(j+i)%4] = wrd[(4*j)+i];   
    } 
  }
 for(byte i=0; i<16; i++){
    wrd[i] = tmp[i]; 
 }
 translate_matrix(wrd);
}

// Wiki
void MixFour(unsigned char * wrd){  
        unsigned char a[4];
        unsigned char b[4];
        unsigned char c;
        unsigned char h;
        /* The array 'a' is simply a copy of the input array 'r'
         * The array 'b' is each element of the array 'a' multiplied by 2
         * in Rijndael's Galois field
         * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
        for(c=0;c<4;c++) {
                a[c] = wrd[c];
                /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
                h = (unsigned char)((signed char)wrd[c] >> 7); /* arithmetic right shift, thus shifting in either zeros or ones */
                b[c] = wrd[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
                b[c] ^= 0x1B & h; /* Rijndael's Galois field */
        }
        wrd[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
        wrd[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
        wrd[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
        wrd[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

byte gmul(byte a, byte b) {
   byte p = 0;
   byte counter;
   byte hi_bit_set;
   for (counter = 0; counter < 8; counter++) {
      if ((b & 1) != 0) {
         p ^= a;
      }
      hi_bit_set = (byte) (a & 0x80);
      a <<= 1;
      if (hi_bit_set != 0) {
         a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
      }
      b >>= 1;
   }
   return p;
}

void InvMixFour(unsigned char * s){
     unsigned char ss[4];
     ss[0] = (gmul(0x0e, s[0]) ^ gmul(0x0b, s[1]) ^ gmul(0x0d,s[2]) ^ gmul(0x09,s[3]));
     ss[1] = (gmul(0x09, s[0]) ^ gmul(0x0e, s[1]) ^ gmul(0x0b,s[2]) ^ gmul(0x0d,s[3]));
     ss[2] = (gmul(0x0d, s[0]) ^ gmul(0x09, s[1]) ^ gmul(0x0e,s[2]) ^ gmul(0x0b,s[3]));
     ss[3] = (gmul(0x0b, s[0]) ^ gmul(0x0d, s[1]) ^ gmul(0x09,s[2]) ^ gmul(0x0e,s[3]));
     for(byte i = 0;i<4;i++)
        s[i] = ss[i];
}
void MixColumns(unsigned char * wrd){
   translate_matrix(wrd);
    unsigned char tmp[4];
    for (byte i = 0; i<4; i++){
      for (byte j = 0; j<4; j++){
        tmp[j] = wrd[j*4+i];
      }
      MixFour(tmp);
      for (byte j = 0; j<4; j++){
        wrd[j*4+i] = tmp[j];
      }
    }
    translate_matrix(wrd);
}
void InvMixColumns(unsigned char * wrd){
   translate_matrix(wrd);
    unsigned char tmp[4];
    for (byte i = 0; i<4; i++){
      for (byte j = 0; j<4; j++){
        tmp[j] = wrd[j*4+i];
      }
      InvMixFour(tmp);
      for (byte j = 0; j<4; j++){
        wrd[j*4+i] = tmp[j];
      }
    }
   translate_matrix(wrd);
}

//http://www.samiam.org/key-schedule.html
void schedule_core(unsigned char *in, unsigned char i) {
        char a;
        /* Rotate the input 8 bits to the left */
        rotate(in);
        /* Apply Rijndael's s-box on all 4 bytes */
        for(a = 0; a < 4; a++) 
                in[a] = sbox(in[a]);
        /* On just the first byte, add 2^i to the byte */
        in[0] ^= rcon(i);
}

//http://www.samiam.org/key-schedule.html
void expand_key(unsigned char *in) { //128 bit to another 176 bytes
        unsigned char t[4];
        /* c is 16 because the first sub-key is the user-supplied key */
        unsigned char c = 16;
        unsigned char i = 1;
        unsigned char a;
        /* We need 11 sets of sixteen bytes each for 128-bit mode */
        while(c < 176) {
                /* Copy the temporary variable over from the last 4-byte
                 * block */
                for(a = 0; a < 4; a++) 
                        t[a] = in[a + c - 4];
                /* Every four blocks (of four bytes), 
                 * do a complex calculation */
                if(c % 16 == 0) {
                    schedule_core(t,i);
                    i++;
                }
                for(a = 0; a < 4; a++) {
                    in[c] = in[c - 16] ^ t[a];
                    c++;
                }
        }
}

void addRoundKey(unsigned char * key, unsigned char * wrd){
  for(byte i=0;i<16;i++){
    wrd[i] = wrd[i] ^ key[i];
  }
}

void InvSubsByte(unsigned char * wrd){
  for (byte i = 0;i<16;i++){
        byte x = wrd[i] & 15;
        byte y = wrd[i] >> 4 & 15;
        byte gtxy = y*16+x;
        wrd[i] = sbox_inv(gtxy);
  }  
}

void SubsByte(unsigned char * wrd){
  for (byte i = 0;i<16;i++){
        byte x = wrd[i] & 15;
        byte y = wrd[i] >> 4 & 15;
        byte gtxy = y*16+x;
        wrd[i] = sbox(gtxy);
  }  
}

unsigned char * AES_encrypt_128(unsigned char * wrd, unsigned char * key, unsigned char * tmp){
    // initial round
    expand_key(key);
    addRoundKey(key, wrd);
    key+=16;
    // rounds
    for(byte i = 0; i<9; i++){
      // sub bytes
      SubsByte(wrd);
      // shift rows
      ShiftRows(wrd,tmp);
      // mix columns
      MixColumns(wrd);
      // mix with keys
      addRoundKey(key, wrd);
      key+=16;
    }
    // final round
    // sub bytes
    SubsByte(wrd);
    // shift rows  
    ShiftRows(wrd,tmp);
    // mix with keys
    addRoundKey(key, wrd);
    //======================
    return wrd;
}
unsigned char * AES_decrypt_128(unsigned char * wrd, unsigned char * key, unsigned char * tmp){
    // initial round
    expand_key(key);
    key += 176;
    key -= 16;
    addRoundKey(key, wrd);
    key -= 16;
    // rounds
    for(byte i = 0; i<9; i++){
      InvShiftRows(wrd,tmp);
      InvSubsByte(wrd);
      addRoundKey(key, wrd);
      key -= 16;     
      InvMixColumns(wrd);
    }
    // final round
    InvShiftRows(wrd,tmp);
    InvSubsByte(wrd);
    addRoundKey(key, wrd);
    return wrd;
}


void setup() {
  Serial.begin(9600);
}

void translate_matrix(unsigned char * mat){
  for(byte i = 0; i < 4 ;i ++)
    for(byte j = i; j < 4 ;j ++){
      if (i != j){
        byte tmp = mat[i*4+j];
        mat[i*4+j] = mat[j*4+i];
        mat[j*4+i] = tmp;
      }  
   }
}
  
void loop() {
  //unsigned char  key[176] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  //unsigned char  key[176] = { 219,19,83,69,   242, 10, 34, 92,   1,1,1,1,    198, 198, 198, 198 };
  //0102030405060708090a0b0c0d0e0f10
  unsigned char  key[176] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  unsigned char  wrd[16] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  unsigned char  tmp[16];
  //unsigned char  tmp[4] = {219, 19, 83, 69};
  //expand_key(key);
  //printExpKey(key);
  //Serial.print("\n");
  //expand_key(key);
  //printExpKey(key);
  //Serial.print("\n");
  /*for(byte i = 0; i<4 ;i++){
    Serial.print(tmp[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
  MixFour(tmp);
  for(byte i = 0; i<4 ;i++){
    Serial.print(tmp[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
  InvMixFour(tmp);
  for(byte i = 0; i<4 ;i++){
    Serial.print(tmp[i]);
    Serial.print(" ");
  }
  Serial.print("\n");*/
  //sbox_field(fild,inv_fild);

  /*printSbox();
  Serial.print("\n");
  printInvSbox();
  Serial.print("\n");*/
/*  delay(1000000);
  ShiftRows(wrd,tmp);
  Serial.print("\n");
  printMatrix(wrd);
  Serial.print("\n");
  delay(100000);*/
  Serial.print("\n");
  printMatrix(wrd);
  Serial.print("\n\n");
  //translate_matrix(key);
  //translate_matrix(wrd);
  AES_encrypt_128(wrd, key, tmp);
  Serial.print("\n");
  printMatrix(wrd);
  Serial.print("\n\n");
  Serial.print("\n\n");
  AES_decrypt_128(wrd, key, tmp);
  Serial.print("\n");
  printMatrix(wrd);
  Serial.print("\n\n");
  delay(1000000);
}
