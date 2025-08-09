#ifndef CHRISTMAS_SONGS_H
#define CHRISTMAS_SONGS_H

#include "pitches.h"

#ifndef REST
#define REST 0
#endif

// Christmas Songs Enum (only includes the songs in getSongData)
enum ChristmasSong {
  JINGLE_BELLS,
  WE_WISH,
  SANTA_CLAUS,
  SILENT_NIGHT,
  RUDOLPH,
  LET_IT_SNOW,
  ALL_I_WANT
};

#define NUM_CHRISTMAS_SONGS 7

// Song Names Array
const char* songNames[] = {
  "Jingle Bells",
  "We Wish You a Merry Christmas",
  "Santa Claus is Coming to Town",
  "Silent Night",
  "Rudolph the Red-Nosed Reindeer",
  "Let It Snow! Let It Snow! Let It Snow!",
  "All I Want for Christmas Is You"
};

// ---
// 1. Jingle Bells (Tempo Adjusted)
const int jingleBells_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

const int jingleBells_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

const int jingleBells_size = sizeof(jingleBells_melody) / sizeof(int);

// ---
// 2. We Wish You a Merry Christmas (Melody & Tempo Corrected)
const int weWish_melody[] = {
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4,
  NOTE_G4, NOTE_G4, REST,
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, REST,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4,
  REST, NOTE_F4
};

const int weWish_tempo[] = {
  4, 8, 8, 8, 8, 8,
  2, 4,
  4, 4, 4, 2,
  4, 4, 4,
  4, 8, 8, 8, 8,
  2, 4,
  4, 4, 4, 4, 4,
  2,
  4, 2
};

const int weWish_size = sizeof(weWish_melody) / sizeof(int);

// ---
// 3. Santa Claus is Coming to Town (Tempo Adjusted)
const int santaClaus_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

const int santaClaus_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

const int santaClaus_size = sizeof(santaClaus_melody) / sizeof(int);

// ---
// 4. Silent Night (Tempo Adjusted)
const int silentNight_melody[] = {
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_D5, NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_C5, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_D5, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_E5,
  NOTE_C5, NOTE_G4, NOTE_E4,
  NOTE_G4, NOTE_F4, NOTE_D4,
  NOTE_C4, NOTE_C4
};

const int silentNight_tempo[] = {
  4, 8, 4, 2,
  4, 8, 4, 2,
  4, 4, 2,
  4, 4, 2,
  4, 4, 8, 8, 4,
  4, 8, 4, 2,
  4, 4, 8, 8, 4,
  4, 8, 4, 2,
  4, 4, 4, 8, 4,
  2, 2,
  4, 4, 4,
  4, 8, 4,
  2, 1
};

const int silentNight_size = sizeof(silentNight_melody) / sizeof(int);
// ---
// 5. Rudolph the Red-Nosed Reindeer (Complete melody)
const int rudolph_melody[] = {
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_B3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_G3,
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_B3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_G3,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_C4, NOTE_A3, NOTE_G3, NOTE_A3, NOTE_B3,
  NOTE_C4
};

const int rudolph_tempo[] = {
  4, 4, 4, 8, 8, 4, 8, 8,
  4, 4, 4, 8, 8, 2,
  8, 8, 8, 8, 4, 4,
  4, 4, 4, 8, 8, 4, 8, 8,
  4, 4, 4, 8, 8, 2,
  8, 8, 8, 8, 4, 4,
  4, 4, 4, 8, 8,
  4, 8, 8, 8, 8,
  4, 8, 8, 8, 8,
  2
};

const int rudolph_size = sizeof(rudolph_melody) / sizeof(int);

// ---
// 6. Let It Snow! Let It Snow! Let It Snow! (Complete melody)
const int letItSnow_melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6,
  NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_F5,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5
};

const int letItSnow_tempo[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2
};

const int letItSnow_size = sizeof(letItSnow_melody) / sizeof(int);

// ---
// 7. All I Want for Christmas Is You (Main recognizable melody)
const int allIWant_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5,
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5
};

const int allIWant_tempo[] = {
  8, 8, 4, 8, 8, 8, 8,
  4, 8, 8, 4, 4,
  4, 8, 8, 4, 4,
  8, 8, 8, 8, 8, 8, 8,
  4, 8, 8, 4, 4,
  8, 8, 8, 8, 8, 8,
  4, 8, 8, 4, 2
};

const int allIWant_size = sizeof(allIWant_melody) / sizeof(int);

// ---
// Function to get song data
struct SongData {
  const int* melody;
  const int* tempo;
  int size;
  int baseTempo;
};

SongData getSongData(ChristmasSong song) {
  switch(song) {
    case JINGLE_BELLS:
      return {jingleBells_melody, jingleBells_tempo, jingleBells_size, 1000};
    case WE_WISH:
      return {weWish_melody, weWish_tempo, weWish_size, 1200};
    case SANTA_CLAUS:
      return {santaClaus_melody, santaClaus_tempo, santaClaus_size, 1100};
    case SILENT_NIGHT:
      return {silentNight_melody, silentNight_tempo, silentNight_size, 1400};
    case RUDOLPH:
      return {rudolph_melody, rudolph_tempo, rudolph_size, 1300};
    case LET_IT_SNOW:
      return {letItSnow_melody, letItSnow_tempo, letItSnow_size, 1200};
    case ALL_I_WANT:
      return {allIWant_melody, allIWant_tempo, allIWant_size, 1100};
  }
}

#endif // CHRISTMAS_SONGS_H