// #define DEBUG

#include <Keypad.h>

typedef struct Note
{
  char key;
  uint16_t value;
} note_t;

typedef struct ChordItem {
  char key;
  int8_t octave;
  bool mute;
} chord_item_t;

typedef struct Chord {
  uint16_t value;
  chord_item_t note1;
  chord_item_t note2;
} chord_t;

const int8_t OCTAVE_MIN = 1;
const int8_t OCTAVE_MAX = 7;
const int8_t OCTAVE_DEFAULT = 4;

const uint16_t BPM_DEFAULT = 180;
const uint16_t INTER_NOTE_DELAY_DEFAULT = 50;

const uint16_t BPM_METRONOME_MIN = 3;
const uint16_t BPM_METRONOME_MAX = UINT16_MAX;
const uint16_t BPM_METRONOME_DEFAULT = 60;

enum class OutPin : byte
{
  One = 10,
  Two = 3,
};

const note_t score_test[] PROGMEM = {
  /** 音階 */
  {'C', 1}, {'c', 1},
  {'D', 1}, {'d', 1},
  {'E', 1},
  {'F', 1}, {'f', 1},
  {'G', 1}, {'g', 1},
  {'A', 1}, {'a', 1},
  {'B', 1},

  {'\0', 1},

  {'B', 1},
  {'a', 1}, {'A', 1},
  {'g', 1}, {'G', 1},
  {'f', 1}, {'F', 1},
  {'E', 1},
  {'d', 1}, {'D', 1},
  {'c', 1}, {'C', 1},

  {'\0', 4},

  /** 音価 */
  {'C', 1},
  {'D', 2},
  {'E', 4},

  {'\0', 4},

  /** 休符 */
  {'A', 1},
  {'\0', 1},
  {'A', 1},
  {'\0', 2},
  {'A', 1},
  {'\0', 4},
  {'A', 1},

  {'\0', 4},

  /** オクターブ */
  {'O', OCTAVE_MIN}, {'A', 1}, /// 1
  {'+', 1}, {'A', 1}, /// 2
  {'+', 1}, {'A', 1}, /// 3
  {'+', 1}, {'A', 1}, /// 4
  {'+', 1}, {'A', 1}, /// 5
  {'+', 1}, {'A', 1}, /// 6
  {'+', 1}, {'A', 1}, /// 7

  {'\0', 1},

  {'O', OCTAVE_MAX}, {'A', 1}, /// 7
  {'-', 1}, {'A', 1}, /// 6
  {'-', 1}, {'A', 1}, /// 5
  {'-', 1}, {'A', 1}, /// 4
  {'-', 1}, {'A', 1}, /// 3
  {'-', 1}, {'A', 1}, /// 2
  {'-', 1}, {'A', 1}, /// 1

  {'O', OCTAVE_DEFAULT},

  {'\0', 4},

  /** BPM */
  {'M', BPM_DEFAULT},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
  {'D', 1}, {'E', 1}, {'D', 2},

  {'\0', 1},

  {'M', BPM_DEFAULT * 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
  {'D', 1}, {'E', 1}, {'D', 2},

  {'\0', 1},

  {'>', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
  {'D', 1}, {'E', 1}, {'D', 2},

  {'\0', 1},

  {'<', 8},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
  {'D', 1}, {'E', 1}, {'D', 2},

  {'\0', 1},

  {'M', UINT16_MAX}, {'_', 0},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'C', 1}, {'D', 1}, {'E', 2},
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
  {'D', 1}, {'E', 1}, {'D', 2},
};

const note_t score_tulip_1[] PROGMEM = {
  {'C', 1}, {'D', 1}, {'E', 2},
};
const note_t score_tulip_2[] PROGMEM = {
  {'G', 1}, {'E', 1}, {'D', 1}, {'C', 1},
};
const note_t score_tulip_3[] PROGMEM = {
  {'D', 1}, {'E', 1}, {'D', 2},
};
const note_t score_tulip_4[] PROGMEM = {
  {'D', 1}, {'E', 1}, {'C', 2},

  {'G', 1}, {'G', 1}, {'E', 1}, {'G', 1},
  {'A', 1}, {'A', 1}, {'G', 2},
  {'E', 1}, {'E', 1}, {'D', 1}, {'D', 1},
  {'C', 4},
};

const note_t score_chouchou[] PROGMEM = {
  {'G', 1}, {'E', 1}, {'E', 2},
  {'F', 1}, {'D', 1}, {'D', 2},
  {'C', 1}, {'D', 1}, {'E', 1}, {'F', 1},
  {'G', 1}, {'G', 1}, {'G', 2},

  {'G', 1}, {'E', 1}, {'E', 1}, {'E', 1},
  {'F', 1}, {'D', 1}, {'D', 1}, {'D', 1},
  {'C', 1}, {'E', 1}, {'G', 1}, {'G', 1},
  {'E', 1}, {'E', 1}, {'E', 2},

  {'D', 1}, {'D', 1}, {'D', 1}, {'D', 1},
  {'D', 1}, {'E', 1}, {'F', 2},
  {'E', 1}, {'E', 1}, {'E', 1}, {'E', 1},
  {'E', 1}, {'F', 1}, {'G', 2},

  {'G', 1}, {'E', 1}, {'E', 1}, {'E', 1},
  {'F', 1}, {'D', 1}, {'D', 1}, {'D', 1},
  {'C', 1}, {'E', 1}, {'G', 1}, {'G', 1},
  {'E', 1}, {'E', 1}, {'E', 1},
};

const chord_t score_chord_test[] PROGMEM = {
  /** @see https://ja.wikipedia.org/wiki/%E5%92%8C%E9%9F%B3#%E5%92%8C%E9%9F%B3%E8%A8%98%E5%8F%B7 */
  /** I */
  {1, {'C',  4, true }, {'E',  4, true }},
  {1, {'E',  4, true }, {'G',  4, true }},
  {1, {'G',  4, true }, {'C',  4, true }},

  /** II */
  {1, {'D',  4, true }, {'F',  4, true }},
  {1, {'F',  4, true }, {'A',  4, true }},
  {1, {'A',  4, true }, {'D',  4, true }},

  /** III */
  {1, {'E',  4, true }, {'G',  4, true }},
  {1, {'G',  4, true }, {'B',  4, true }},
  {1, {'B',  4, true }, {'E',  4, true }},

  /** IV */
  {1, {'F',  4, true }, {'A',  4, true }},
  {1, {'A',  4, true }, {'C',  5, true }},
  {1, {'C',  5, true }, {'F',  4, true }},

  /** V */
  {1, {'G',  4, true }, {'B',  4, true }},
  {1, {'B',  4, true }, {'D',  5, true }},
  {1, {'D',  5, true }, {'G',  4, true }},

  /** VI */
  {1, {'A',  4, true }, {'C',  5, true }},
  {1, {'C',  5, true }, {'E',  5, true }},
  {1, {'E',  5, true }, {'A',  4, true }},

  /** VII */
  {1, {'B',  4, true }, {'D',  5, true }},
  {1, {'D',  5, true }, {'F',  5, true }},
  {1, {'F',  5, true }, {'B',  4, true }},

  {1, {'\0', 0, true }, {'\0', 0, true }},

  {1, {'A',  4, false}, {'A',  1, true }},
  {1, {'A',  4, false}, {'A',  2, true }},
  {1, {'A',  4, false}, {'A',  3, true }},
  {1, {'A',  4, false}, {'A',  4, true }},
  {1, {'A',  4, false}, {'A',  5, true }},
  {1, {'A',  4, false}, {'A',  6, true }},
  {1, {'A',  4, false}, {'A',  7, true }},

  {1, {'\0', 0, true }, {'\0', 0, true }},

  {1, {'C',  1, true }, {'C',  4, false}},
  {1, {'C',  2, true }, {'C',  4, false}},
  {1, {'C',  3, true }, {'C',  4, false}},
  {1, {'C',  4, true }, {'C',  4, false}},
  {1, {'C',  5, true }, {'C',  4, false}},
  {1, {'C',  6, true }, {'C',  4, false}},
  {1, {'C',  7, true }, {'C',  4, false}},
};

/** https://en.wikipedia.org/wiki/Toccata_and_Fugue_in_D_minor,_BWV_565 */
const chord_t score_chord_toccata_and_fugue[] PROGMEM = {
  {8, {'A',  5, true }, {'A',  4, true }},

  {2, {'\0', 5, true }, {'\0', 4, true }},

  {1, {'G',  5, true }, {'G',  4, true }},
  {1, {'F',  5, true }, {'F',  4, true }},
  {1, {'E',  5, true }, {'E',  4, true }},
  {1, {'D',  5, true }, {'D',  4, true }},
  {2, {'c',  5, true }, {'c',  4, true }},
  {4, {'D',  5, true }, {'D',  4, true }},

  {4, {'\0', 5, true }, {'\0', 4, true }},
  {8, {'\0', 5, true }, {'\0', 4, true }},

  {8, {'A',  4, true }, {'A',  3, true }},

  {2, {'\0', 4, true }, {'\0', 3, true }},

  {2, {'E',  4, true }, {'E',  3, true }},
  {2, {'F',  4, true }, {'F',  3, true }},
  {2, {'c',  4, true }, {'c',  3, true }},
  {4, {'D',  4, true }, {'D',  3, true }},

  {4, {'\0', 5, true }, {'\0', 4, true }},
  {8, {'\0', 5, true }, {'\0', 4, true }},

  {8, {'A',  3, true }, {'A',  2, true }},

  {2, {'\0', 3, true }, {'\0', 2, true }},

  {1, {'G',  3, true }, {'G',  2, true }},
  {1, {'F',  3, true }, {'F',  2, true }},
  {1, {'E',  3, true }, {'E',  2, true }},
  {1, {'D',  3, true }, {'D',  2, true }},
  {2, {'c',  3, true }, {'c',  2, true }},
  {2, {'D',  3, true }, {'D',  2, true }},

  {2, {'\0', 3, true }, {'\0', 2, true }},
  {8, {'\0', 3, true }, {'\0', 2, true }},

  {4, {'<'}},

  {4, {'\0', 3, true }, {'D',  2, true }},
  {4, {'\0', 3, true }, {'c',  3, true }},
  {4, {'\0', 3, true }, {'E',  3, true }},
  {4, {'\0', 3, true }, {'G',  3, true }},
  {4, {'B',  3, true }, {'\0', 3, true }},
  {4, {'c',  4, true }, {'\0', 3, true }},
  {4, {'E',  4, true }, {'\0', 3, true }},
};

/** https://i.pinimg.com/originals/62/58/a2/6258a2863b8491125d6d076a95e234a5.jpg */
const chord_t score_chord_twinkle_star_1[] PROGMEM = {
  {1, {'C',  4, true }, {'C',  3, false}},
  {1, {'C',  4, true }, {'\0', 3, true }},
  {1, {'G',  4, true }, {'G',  3, false}},
  {1, {'G',  4, true }, {'\0', 3, true }},

  {1, {'A',  4, true }, {'F',  3, false}},
  {1, {'A',  4, true }, {'\0', 3, true }},
  {2, {'G',  4, true }, {'C',  3, true }},

  {1, {'F',  4, true }, {'F',  3, false}},
  {1, {'F',  4, true }, {'\0', 3, true }},
  {1, {'E',  4, true }, {'C',  3, false}},
  {1, {'E',  4, true }, {'\0', 3, true }},

  {1, {'D',  4, true }, {'G',  3, false}},
  {1, {'D',  4, true }, {'\0', 3, true }},
  {2, {'C',  4, true }, {'C',  3, true }},
};
const chord_t score_chord_twinkle_star_2[] PROGMEM = {
  {1, {'G',  4, true }, {'C',  3, false}},
  {1, {'G',  4, true }, {'\0', 3, true }},
  {1, {'F',  4, true }, {'F',  3, false}},
  {1, {'F',  4, true }, {'\0', 3, true }},

  {1, {'E',  4, true }, {'C',  3, false}},
  {1, {'E',  4, true }, {'\0', 3, true }},
  {2, {'D',  4, true }, {'G',  3, true }},
};

char pitches[3] = {'\0', '\0', '\0'};
int8_t octave = OCTAVE_DEFAULT;
uint16_t bpm = BPM_DEFAULT;
uint16_t interNoteDelay = INTER_NOTE_DELAY_DEFAULT;
uint16_t bpm_metronome = BPM_METRONOME_DEFAULT;

/// for Custom Key Matrix
const byte ROWS = 4; /// 4行のキーパッドを使用
const byte COLS = 4; /// 4列のキーパッドを使用

char keyMatrix[COLS][ROWS] = {
  {'C','c','D','d'},
  {'E','F','f','G'},
  {'g','A','a','B'},
  {'-','0','+','~'},
};

byte rowPins[ROWS] = {7, 6, 4, 2}; /// connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 11, 12}; /// connect to the column pinouts of the keypad

Keypad kpd = Keypad(makeKeymap(keyMatrix), rowPins, colPins, ROWS, COLS);

#if defined(DEBUG)
void SerialPrintHr()
{
  Serial.println("-------------------------------");
}
#endif

bool isPitchKey(char key)
{
  switch(key) {
    case 'C': case 'c':
    case 'D': case 'd':
    case 'E':
    case 'F': case 'f':
    case 'G': case 'g':
    case 'A': case 'a':
    case 'B':
      return true;

    default:
      return false;
  }
}

bool isUpDownKey(char key)
{
  switch(key) {
    case '-':
    case '+':
    case '0':
      return true;

    default:
      return false;
  }
}

bool isFunctionKey(char key)
{
  switch(key) {
    case '~':
      return true;

    default:
      return false;
  }
}

int8_t getPitchNum(char pitch)
{
  switch (pitch)
  {
    case 'C': return 0;
    case 'c': return 1;
    case 'D': return 2;
    case 'd': return 3;
    case 'E': return 4;
    case 'F': return 5;
    case 'f': return 6;
    case 'G': return 7;
    case 'g': return 8;
    case 'A': return 9;
    case 'a': return 10;
    case 'B': return 11;

    default: return -1;
  }
}

void setOctave(int8_t newOctave)
{
  octave = newOctave < OCTAVE_MIN ? OCTAVE_MIN
    : newOctave > OCTAVE_MAX ? OCTAVE_MAX
    : newOctave;

  #if defined(DEBUG)
  Serial.print("octave: "); Serial.print(octave);
  Serial.println();
  SerialPrintHr();
  #endif
}

void setBpm(uint16_t newBpm) {
  bpm = newBpm;
  interNoteDelay = INTER_NOTE_DELAY_DEFAULT * BPM_DEFAULT / newBpm;

  #if defined(DEBUG)
  Serial.print("bpm: "); Serial.print(bpm); Serial.print(", ");
  Serial.print("interNoteDelay: "); Serial.print(interNoteDelay);
  Serial.println();
  SerialPrintHr();
  #endif
}

void setInterNoteDelay(uint16_t newInterNoteDelay) {
  interNoteDelay = newInterNoteDelay;

  #if defined(DEBUG)
  Serial.print("interNoteDelay: "); Serial.print(interNoteDelay);
  Serial.println();
  SerialPrintHr();
  #endif
}

void setBpmMetronome(uint16_t newBpmMetronome) {
  bpm_metronome = newBpmMetronome < BPM_METRONOME_MIN ? BPM_METRONOME_MIN
    : newBpmMetronome > BPM_METRONOME_MAX ? BPM_METRONOME_MAX
    : newBpmMetronome;

  #if defined(DEBUG)
  Serial.print("bpm metronome: "); Serial.print(bpm_metronome); Serial.print(", ");
  Serial.println();
  SerialPrintHr();
  #endif
}

const uint8_t beep_table_size = (OCTAVE_MAX - OCTAVE_MIN + 1) * 12;
const uint16_t tops_1[beep_table_size] PROGMEM = {
  30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198,
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 64793,
  61156, 57724, 54484, 51426, 48540, 45815, 43244, 40817, 38526, 36364, 34323, 32396,
  30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198,
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099,
  7645, 7215, 6810, 6428, 6067, 5727, 5405, 5102, 602, 4545, 4290, 4050,
  3822, 451, 3405, 3214, 3034, 2863, 2703, 2551, 301, 2273, 2145, 2025,
};
const uint8_t prescalers_1[beep_table_size] PROGMEM = {
  0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010,
  0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b001,
  0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001,
  0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001,
  0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001,
  0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b010, 0b001, 0b001, 0b001,
  0b001, 0b010, 0b001, 0b001, 0b001, 0b001, 0b001, 0b001, 0b010, 0b001, 0b001, 0b001,
};
const uint8_t tops_2[beep_table_size] PROGMEM = {
  239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127,
  119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 253,
  239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 253,
  239, 225, 213, 201, 95, 179, 169, 159, 150, 142, 134, 253,
  239, 225, 213, 201, 95, 179, 169, 159, 150, 142, 134, 253,
  239, 225, 213, 201, 95, 179, 169, 159, 150, 142, 134, 127,
  119, 113, 106, 100, 95, 89, 21, 80, 75, 71, 67, 253,
};
const uint8_t prescalers_2[beep_table_size] PROGMEM = {
  0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111,
  0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b110,
  0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b110, 0b101,
  0b101, 0b101, 0b101, 0b101, 0b110, 0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 0b100,
  0b100, 0b100, 0b100, 0b100, 0b101, 0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b011,
  0b011, 0b011, 0b011, 0b011, 0b100, 0b011, 0b011, 0b011, 0b011, 0b011, 0b011, 0b011,
  0b011, 0b011, 0b011, 0b011, 0b011, 0b011, 0b101, 0b011, 0b011, 0b011, 0b011, 0b010,
};

void beep(OutPin pin, char pitch, int8_t octaveTmp = INT8_MIN)
{
  int8_t _octave = octaveTmp != INT8_MIN
    ? octaveTmp
    : octave;

  _octave = _octave < OCTAVE_MIN ? OCTAVE_MIN
    : _octave > OCTAVE_MAX ? OCTAVE_MAX
    : _octave;

  uint8_t tableIndex = (_octave - OCTAVE_MIN) * 12 + getPitchNum(pitch);

  uint16_t top;
  uint8_t prescaler;

  switch (pin) {
    case OutPin::One:
      top = pgm_read_word_near(&tops_1[tableIndex]);
      prescaler = pgm_read_byte_near(&prescalers_1[tableIndex]);

      TCCR1B |= (1<<WGM13) | prescaler;
      OCR1A = top;
      OCR1B = top / 2;
    break;

    case OutPin::Two:
      top = (uint16_t) pgm_read_byte_near(&tops_2[tableIndex]);
      prescaler = pgm_read_byte_near(&prescalers_2[tableIndex]);

      TCCR2B |= (1<<WGM22) | prescaler;
      OCR2A = top;
      OCR2B = top / 2;
    break;
  }

  #if defined(DEBUG)
  Serial.print("pin: "); Serial.print((int) pin);
  Serial.print(", ");
  Serial.print("timer: "); Serial.print(pin == OutPin::One ? 1 : pin == OutPin::Two ? 2 : NULL);
  Serial.println();

  Serial.print("top: "); Serial.print(top);
  Serial.print(", ");
  Serial.print("prescaler: "); Serial.print(prescaler);
  Serial.print(", ");
  Serial.print("idx: "); Serial.print(tableIndex);
  Serial.println();

  Serial.print("pitch: "); Serial.print(pitch);
  Serial.print(", ");
  Serial.print("octave: "); Serial.print(_octave);
  Serial.print(", ");
  Serial.print("bpm: "); Serial.print(bpm);
  Serial.println();

  SerialPrintHr();
  #endif
}

void mute(OutPin pin)
{
  switch (pin)
  {
    case OutPin::One:
      TCCR1B = 0x00;
    break;

    case OutPin::Two:
      TCCR2B = 0x00;
    break;
  }
}

bool autoPlayKeysHandle(uint16_t bpm_init)
{
  if (kpd.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) { /// Scan the whole key list.
      if ( kpd.key[i].stateChanged ) { /// Only find keys that have changed state.
        char key = kpd.key[i].kchar;

        switch (kpd.key[i].kstate) { /// Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (isFunctionKey(key)) {
              return true;
            } else if (isUpDownKey(key)) {
              switch (key) {
                case '+':
                  setBpm(bpm * 2);
                break;

                case '-':
                  setBpm(bpm / 2);
                break;

                case '0':
                  setBpm(bpm_init);
                break;
              }
            } else if (isPitchKey(key)) {
              uint8_t pitchNum = getPitchNum(key);
              setOctave(OCTAVE_MIN + pitchNum);
            }
          break;

          case HOLD:
          case RELEASED:
          case IDLE:
          break;
        }
      }
    }
  }

  return false;
}

void autoBeep(char pitch, uint8_t noteValue = 1, OutPin pin = OutPin::One)
{
  if (pitch) {
    beep(pin, pitch);
  }
  delay((60000 / bpm) * noteValue);
  mute(pin);
}

bool autoPlayScore(const note_t score[], uint8_t size)
{
  int8_t octave_init = octave;
  int16_t bpm_init = bpm;

  bool isCanceled = false;

  for (int i = 0; i < size; i++) {
    note_t note;
    note.key = pgm_read_byte_near(&score[i].key);
    note.value = pgm_read_word_near(&score[i].value);

    if (note.key == 'M') {
      setBpm(note.value);
      continue;
    }

    if (note.key == '<') {
      setBpm(bpm / note.value);
      continue;
    }

    if (note.key == '>') {
      setBpm(bpm * note.value);
      continue;
    }

    if (note.key == '_') {
      setInterNoteDelay(note.value);
      continue;
    }

    if (note.key == 'O' || isUpDownKey(note.key)) {
      switch (note.key)
      {
        case 'O':
          setOctave(note.value);
        break;

        case '-':
          setOctave(octave - note.value);
        break;

        case '+':
          setOctave(octave + note.value);
        break;

        case '0':
          setOctave(octave_init);
        break;
      }

      continue;
    }

    autoBeep(note.key, note.value);
    delay(interNoteDelay);

    isCanceled = autoPlayKeysHandle(bpm_init);
    if (isCanceled) break;
  }

  return isCanceled;
}

bool autoPlayScoreChord(const chord_t score[], uint8_t size)
{
  int16_t bpm_init = bpm;

  bool isCanceled = false;

  for (int i = 0; i < size; i++) {
    chord_t chord;

    chord.value = pgm_read_word_near(&score[i].value);

    chord.note1.key = pgm_read_byte_near(&score[i].note1.key);
    chord.note1.octave = pgm_read_byte_near(&score[i].note1.octave);
    chord.note1.mute = pgm_read_byte_near(&score[i].note1.mute);

    chord.note2.key = pgm_read_byte_near(&score[i].note2.key);
    chord.note2.octave = pgm_read_byte_near(&score[i].note2.octave);
    chord.note2.mute = pgm_read_byte_near(&score[i].note2.mute);

    if (chord.note1.key == 'M') {
      setBpm(chord.value);
      continue;
    }

    if (chord.note1.key == '<') {
      setBpm(bpm / chord.value);
      continue;
    }

    if (chord.note1.key == '>') {
      setBpm(bpm * chord.value);
      continue;
    }

    uint16_t noteValue = 60000 / bpm;
    uint16_t interNoteDelay = INTER_NOTE_DELAY_DEFAULT * BPM_DEFAULT / bpm;

    if (chord.note1.key) beep(OutPin::One, chord.note1.key, octave + chord.note1.octave - OCTAVE_DEFAULT);
    if (chord.note2.key) beep(OutPin::Two, chord.note2.key, octave + chord.note2.octave - OCTAVE_DEFAULT);
    delay(noteValue * chord.value);
    if (chord.note1.mute) mute(OutPin::One);
    if (chord.note2.mute) mute(OutPin::Two);
    delay(interNoteDelay);

    isCanceled = autoPlayKeysHandle(bpm_init);
    if (isCanceled) break;
  }

  mute(OutPin::One);
  mute(OutPin::Two);

  return isCanceled;
}

void autoPlay()
{
  int8_t octave_org = octave;
  uint16_t bpm_org = bpm;
  uint16_t interNoteDelay_org = interNoteDelay;

  long rand = random();
  bool isCanceled = false;

  setOctave(OCTAVE_DEFAULT);

  switch (octave_org) {
    case OCTAVE_DEFAULT - 2:
      isCanceled = autoPlayScore(score_chouchou, sizeof(score_chouchou) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
    break;

    case OCTAVE_DEFAULT - 1:
      isCanceled = autoPlayScore(score_tulip_1, sizeof(score_tulip_1) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_1, sizeof(score_tulip_1) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_2, sizeof(score_tulip_2) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_3, sizeof(score_tulip_3) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_1, sizeof(score_tulip_1) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_1, sizeof(score_tulip_1) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_2, sizeof(score_tulip_2) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScore(score_tulip_4, sizeof(score_tulip_4) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
    break;

    case OCTAVE_DEFAULT:
      isCanceled = autoPlayScoreChord(score_chord_test, sizeof(score_chord_test) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
      delay(1000);
      isCanceled = autoPlayScore(score_test, sizeof(score_test) / sizeof(note_t)); if (isCanceled) goto autoPlay_exit;
    break;

    case OCTAVE_DEFAULT + 1:
      isCanceled = autoPlayScoreChord(score_chord_twinkle_star_1, sizeof(score_chord_twinkle_star_1) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScoreChord(score_chord_twinkle_star_2, sizeof(score_chord_twinkle_star_2) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScoreChord(score_chord_twinkle_star_2, sizeof(score_chord_twinkle_star_2) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
      isCanceled = autoPlayScoreChord(score_chord_twinkle_star_1, sizeof(score_chord_twinkle_star_1) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
    break;

    default:
      setBpm(600);
      isCanceled = autoPlayScoreChord(score_chord_toccata_and_fugue, sizeof(score_chord_toccata_and_fugue) / sizeof(chord_t)); if (isCanceled) goto autoPlay_exit;
    break;
  }

  autoPlay_exit:

  setOctave(octave_org);
  setBpm(bpm_org);
  setInterNoteDelay(interNoteDelay_org);
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite((uint8_t) OutPin::One, !digitalRead((uint8_t) OutPin::One));
}

void setupSynth() {
  noInterrupts();

  /** Timer 1 Phase Correct PWM, Mode: 11, TOP: OCR1A */
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  TCCR1A |= (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
  TCCR1B |= (1<<WGM13);

  OCR1B = 0;

  /** Timer 2 Phase Correct PWM, Mode: 5, TOP: OCR2A */
  TCCR2A = 0x00;
  TCCR2B = 0x00;

  TCCR2A |= (1<<COM2B1) | (1<<WGM20);
  TCCR2B |= (1<<WGM22);

  OCR2B = 0;

  interrupts();
}

void setupMetronome()
{
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 15625.0 * 60.0 / bpm_metronome;

  /** prescaler: 1024 */
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);

  /** CTC: Mode 4 */
  TCCR1B |= (1 << WGM12);

  /** enable ISR(TIMER1_COMPA_vect) */
  TIMSK1 |= (1 << OCIE1A);

  interrupts();
}

void cleanupMetronome()
{
  TIMSK1 = 0;
  digitalWrite((uint8_t) OutPin::One, LOW);
  setupSynth();
}

void metronome()
{
  setupMetronome();

  char holdKey = '\0';

  while (1) {
    char key = kpd.getKey();
    KeyState KeyState = kpd.getState();

    if (key) holdKey = key;

    if (KeyState == PRESSED || KeyState == HOLD) {
      switch (holdKey) {
        case '~':
          goto metronome_exit;
        break;

        case '-':
          setBpmMetronome(bpm_metronome - 3);
        break;

        case '+':
          setBpmMetronome(bpm_metronome + 3);
        break;

        case '0':
          setBpmMetronome(BPM_METRONOME_DEFAULT);
        break;

        default:
        break;
      }

      setupMetronome();
    }

    delay(100);
  }

  metronome_exit:
  cleanupMetronome();
}

void setup() {
  pinMode((byte) OutPin::One, OUTPUT);
  pinMode((byte) OutPin::Two, OUTPUT);

  setupSynth();

  #if defined(DEBUG)
  Serial.begin(9600);
  Serial.println("===========================");
  Serial.println("SETUP DONE");
  Serial.println("===========================");
  #endif

  beep(OutPin::One, 'A');
  delay(500);
  mute(OutPin::One);
}

void loop() {
  if (kpd.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) { /// Scan the whole key list.
      if ( kpd.key[i].stateChanged ) { /// Only find keys that have changed state.
        char key = kpd.key[i].kchar;

        switch (kpd.key[i].kstate) { /// Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (isPitchKey(key)) {
              for (int p = 1; p <= 2; p++) {
                if (pitches[p]) continue;
                pitches[p] = key;
                switch (p) {
                  case 1:
                    beep(OutPin::One, key);
                  break;

                  case 2:
                    beep(OutPin::Two, key);
                  break;
                }

                break;
              }
            } else if (isUpDownKey(key)) {
              switch (key) {
                case '-':
                  setOctave(octave - 1);
                break;

                case '+':
                  setOctave(octave + 1);
                break;

                case '0':
                  setOctave(OCTAVE_DEFAULT);
                break;
              }
            } else if (isFunctionKey(key)) {
              char mode;
              bool hasReleased;
              unsigned long t;

              mode = '\0';
              hasReleased = false;
              t = millis();

              char modeKey;
              KeyState modeKeyState;

              while(millis() - t < 1000) {
                modeKey = kpd.getKey();
                modeKeyState = kpd.getState();

                if (modeKey) {
                  mode = modeKey;
                }

                switch (modeKeyState)
                {
                  case PRESSED:
                  case HOLD:
                  break;

                  case RELEASED:
                  case IDLE:
                  default:
                    hasReleased = true;
                  break;
                }
              }

              if (hasReleased) {
                switch (mode)
                {
                  default:
                    metronome();
                  break;
                }
              } else {
                autoPlay();
              }
            }
          break;

          case RELEASED:
            if (isPitchKey(key)) {
              for (int p = 1; p <= 2; p++) {
                if (!pitches[p]) continue;

                if (pitches[p] == key) {
                  pitches[p] = '\0';
                  switch (p) {
                    case 1:
                      mute(OutPin::One);
                    break;

                    case 2:
                      mute(OutPin::Two);
                    break;
                  }

                  break;
                }
              }
            }
          break;

          case HOLD:
          case IDLE:
          break;
        }
      }
    }
  }
}

