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

// My work! beranm14
void printMatrix(unsigned char * wrd){
  for(byte i=0; i<4; i++){
    for(byte j=0; j<4; j++){   
      Serial.print(((byte)wrd[(4*i)+j]));
      Serial.print(" "); 
    } 
    Serial.print("\n");
  }
}

// My work! beranm14
void ShiftRows(unsigned char * wrd,unsigned char * tmp){
  for(byte i=0; i<4; i++){
    for(byte j=0; j<4; j++){
      tmp[(4*i)+j] = wrd[(4*i)+(j+i)%4];   
    } 
  }
}

// Wiki
void MixColumns(unsigned char * wrd){
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
unsigned char i; // needs to be reset, not enough memory for 172 Bytes, therefore in expand key we'll get only 128 new bits of key
void expand_key(unsigned char *in, unsigned char *out) { //128 bit to another 128 bits
        unsigned char t[4];
        /* c is 16 because the first sub-key is the user-supplied key */
        unsigned char c = 16;
        unsigned char a;

        /* We need 11 sets of sixteen bytes each for 128-bit mode */
        while(c < 32 /*< 176*/) { // change to just another 128 bits
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
                        out[c-16] = in[c - 16] ^ t[a];
                        c++;
                }
        }
}

unsigned char * AES_encrypt_128(unsigned char * wrd, unsigned char * key, unsigned char * tmp){
    // i for key expansion
    i = 1;
    unsigned char  tmp_key[22];
    //
    printMatrix(wrd);
    expand_key(key,tmp_key);
    // initial round
    add_round_key(key,);
    // rounds

    // final round
    /*for (byte i = 0;i<16;i++){
      wrd[i] = sbox(wrd[i]);
    }*/
    ShiftRows(wrd,tmp);
    Serial.print("\n");
    printMatrix(tmp);
    Serial.print("\n\n");
    MixColumns(tmp);
    Serial.print("\n\n");
}

void setup() {
  Serial.begin(9600);
}



void loop() {
  unsigned char  key[22] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  unsigned char  wrd[16] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
  unsigned char  tmp[16] = {};
  AES_encrypt_128(wrd, key, tmp);
  // put your main code here, to run repeatedly:
  delay(100000);
}
