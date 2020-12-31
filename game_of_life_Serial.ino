/*
  Arduino Serial game of life.
  Game of life running on arduino serial communication.
  (C)2021 Kei Takagi

  ビット操作バージョン
*/

#include <limits.h> //CHAR_BITの定義獲得
#define WT 160       //WIDTH
#define HT 72        //HEIGHT
#define MapSize(val)    (((val) + CHAR_BIT - 1) / CHAR_BIT)

unsigned char area[MapSize(HT * WT)];
unsigned char wk[MapSize(4 * WT)];
char txt[WT + 1] = {0};
unsigned char tmp;
int i, j, k, p;

void setup() {
  randomSeed(analogRead(0));

  Serial.begin(115200);
  Serial.print("\033[2J");//全画面消去

  for (i = 0; i < HT; i++) {
    p = WT * i;
    for (j = 0; j < WT; j++)
      setBit(area, p + j, rand() % 2 == 0 ? false : true);
  }
}

void loop() {
  Serial.print("\033[0;0H");//座標移動

  for (i = 0; i < HT; i++) {
    p = WT * i;
    for (j = 0; j < WT; j++)
      txt[j] = getBit(area, p + j) ? 'O' : ' ';
    Serial.println(txt);
  }

  for (i = 0; i < 2; i++) {
    p = WT * i;
    for (j = 0; j < WT; j++)
      setBit(wk, p + j,  getBit( area, WT * (i == 0 ? HT - 1 : i - 1) + j));
  }

  for (j = 0; j < WT; j++)
    setBit(wk, WT * 3 + j,  getBit( area, j));

  for (i = 0; i < HT; i++) {
    if (i == (HT - 1)) {
      for (j = 0; j < WT; j++)
        setBit(wk, WT * 2 + j,  getBit( wk, WT * 3 + j));
    } else {
      for (j = 0; j < WT; j++)
        setBit(wk, WT * 2 + j,  getBit( area, (WT * (i + 1)) + j));
    }

    for (j = 0; j < WT; j++) {
      tmp = getBit( wk, j == 0 ? WT - 1 : j - 1) +
            getBit( wk, j ) +
            getBit( wk, j + 1 < WT ? j + 1 : 0 )  +
            getBit( wk, WT + ( j == 0 ? WT - 1 : j - 1))  +
            getBit( wk, WT + ( j + 1 < WT ? j + 1 : 0 ))  +
            getBit( wk, WT * 2 + (j == 0 ? WT - 1 : j - 1)) +
            getBit( wk, WT * 2 + (j )) +
            getBit( wk, WT * 2 + (j + 1 < WT ? j + 1 : 0 )) ;

      p = WT * i + j;
      if ( getBit(area, p) == true ) {
        setBit(area , p , (tmp == 2 || tmp == 3) );
      } else {
        setBit(area , p , (tmp == 3) );
      }
    }

    for (k = 0; k < 2; k++) {
      p = WT * k;
      for (j = 0; j < WT; j++)
        setBit(wk, p + j,  getBit( wk, p + WT + j));
    }
  }
}

// 指定した場所のビットに true か false をセット
inline void setBit(unsigned char* p, unsigned int index, bool v)
{
  if (v == true) {
    p[index / CHAR_BIT] |= (1 << (index % CHAR_BIT));
  } else {
    p[index / CHAR_BIT] &= ~(1 << (index % CHAR_BIT));
  }
}

// 指定した場所のビット状態を true か false で返す
inline bool getBit(const unsigned char* p, unsigned int index)
{
  return p[index / CHAR_BIT] & (1 << (index % CHAR_BIT));
}
