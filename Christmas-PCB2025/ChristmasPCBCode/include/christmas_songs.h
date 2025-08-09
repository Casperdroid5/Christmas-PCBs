#ifndef CHRISTMAS_SONGS_H
#define CHRISTMAS_SONGS_H

#include "pitches.h"  // Use your existing pitches file

#ifndef REST
#define REST 0
#endif

// Christmas Songs Enum
enum ChristmasSong {
  JINGLE_BELLS,
  WE_WISH,
  SANTA_CLAUS,
  SILENT_NIGHT,
  DECK_THE_HALLS,
  O_COME_ALL_YE_FAITHFUL,
  THE_FIRST_NOEL,
  JOY_TO_THE_WORLD,
  HARK_THE_HERALD,
  ANGELS_WE_HAVE_HEARD,
  GOOD_KING_WENCESLAS,
  O_HOLY_NIGHT,
  RUDOLPH,
  LET_IT_SNOW,
  ALL_I_WANT
};

#define NUM_CHRISTMAS_SONGS 12

// Song Names Array
const char* songNames[] = {
  "Jingle Bells",
  "We Wish You a Merry Christmas", 
  "Santa Claus is Coming to Town",
  "Silent Night",
  "Deck the Halls",
  "O Come All Ye Faithful",
  "The First Noel",
  "Joy to the World",
  "Hark! The Herald Angels Sing",
  "Angels We Have Heard on High",
  "Good King Wenceslas",
  "O Holy Night",
  "Rudolph the Red-Nosed Reindeer",
  "Let It Snow! Let It Snow! Let It Snow!",
  "All I Want for Christmas Is You"
};

// 1. Jingle Bells - Verified melody
const int jingleBells_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

const int jingleBells_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

const int jingleBells_size = sizeof(jingleBells_melody) / sizeof(int);

// 2. We Wish You a Merry Christmas - Fixed from reliable source
const int weWish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

const int weWish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

const int weWish_size = sizeof(weWish_melody) / sizeof(int);

// 3. Santa Claus is Coming to Town
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

// 4. Silent Night - Complete accurate version from robsoncouto
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

// 5. Deck the Halls - Fixed accurate melody
const int deckHalls_melody[] = {
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, 
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4, 
  NOTE_E4, 
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, 
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_D4, NOTE_C4, NOTE_B3, NOTE_A3, 
  NOTE_G3,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, 
  NOTE_D4, NOTE_D4, NOTE_E4, NOTE_C4,
  NOTE_G4
};

const int deckHalls_tempo[] = {
  8, 8, 8, 8,
  4, 4, 4, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  4, 4, 4, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 4, 4,
  2
};

const int deckHalls_size = sizeof(deckHalls_melody) / sizeof(int);

// 6. O Come All Ye Faithful - Traditional accurate melody
const int oComeAllYe_melody[] = {
  NOTE_G4, NOTE_G4, NOTE_D5, NOTE_D5,
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_G4,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_F5, NOTE_E5,
  NOTE_D5, NOTE_C5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_A4, NOTE_G4
};

const int oComeAllYe_tempo[] = {
  4, 4, 4, 4,
  4, 8, 8, 4,
  4, 4, 8, 8,
  4, 4, 2,
  4, 8, 8, 4,
  4, 4, 2,
  4, 4, 4, 8, 8,
  4, 4, 4, 8, 8,
  4, 4, 1
};

const int oComeAllYe_size = sizeof(oComeAllYe_melody) / sizeof(int);

// 7. The First Noel - Traditional melody
const int firstNoel_melody[] = {
  NOTE_D4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4,
  NOTE_A4, NOTE_F4, NOTE_D4,
  NOTE_D4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4,
  NOTE_A4, NOTE_F4, NOTE_D4,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
  NOTE_B4, NOTE_G4, NOTE_D4
};

const int firstNoel_tempo[] = {
  4, 8, 8, 4,
  4, 4, 8, 8,
  4, 4, 8, 8,
  4, 4, 2,
  4, 8, 8, 4,
  4, 4, 8, 8,
  4, 4, 8, 8,
  4, 4, 2,
  4, 8, 8, 4,
  4, 4, 8, 8,
  4, 4, 8, 8,
  4, 4, 2
};

const int firstNoel_size = sizeof(firstNoel_melody) / sizeof(int);

// 8. Joy to the World - Classic descending melody
const int joyToWorld_melody[] = {
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_D5,
  NOTE_D5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_D5, NOTE_G4, NOTE_D4
};

const int joyToWorld_tempo[] = {
  4, 8, 8, 8, 8,
  4, 8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  8, 8, 8, 8,
  4, 4, 1
};

const int joyToWorld_size = sizeof(joyToWorld_melody) / sizeof(int);

// 9. Hark! The Herald Angels Sing - Traditional melody
const int harkHerald_melody[] = {
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_E4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_C4,
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_E4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_C4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4,
  NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4,
  NOTE_B4, NOTE_A4, NOTE_G4
};

const int harkHerald_tempo[] = {
  4, 8, 8, 4,
  4, 4, 2,
  4, 4, 8, 8,
  4, 4, 2,
  4, 8, 8, 4,
  4, 4, 2,
  4, 4, 8, 8,
  4, 4, 2,
  4, 4, 8, 8,
  4, 4, 2,
  4, 4, 8, 8,
  4, 4, 1
};

const int harkHerald_size = sizeof(harkHerald_melody) / sizeof(int);

// 10. Angels We Have Heard on High - Traditional with Gloria
const int angelsHeard_melody[] = {
  NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4,
  NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4,
  NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4,
  NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4,
  // Gloria section - simplified
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_E4,
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5
};

const int angelsHeard_tempo[] = {
  4, 8, 8, 4,
  4, 4, 2,
  4, 4, 8, 8,
  4, 2,
  4, 8, 8, 4,
  4, 4, 2,
  4, 4, 8, 8,
  4, 2,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 1
};

const int angelsHeard_size = sizeof(angelsHeard_melody) / sizeof(int);

// 11. Good King Wenceslas - Traditional melody
const int goodKing_melody[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_D4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4,
  NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_D4
};

const int goodKing_tempo[] = {
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 1
};

const int goodKing_size = sizeof(goodKing_melody) / sizeof(int);

// 12. O Holy Night - Traditional melody (simplified)
const int oHolyNight_melody[] = {
  NOTE_C4, NOTE_F4, NOTE_F4,
  NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4,
  NOTE_AS4, NOTE_A4, NOTE_G4,
  NOTE_F4,
  NOTE_C4, NOTE_F4, NOTE_F4,
  NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_C5, NOTE_AS4,
  NOTE_A4,
  NOTE_F4, NOTE_G4, NOTE_A4,
  NOTE_C5, NOTE_F4,
  NOTE_G4, NOTE_F4
};

const int oHolyNight_tempo[] = {
  4, 4, 8, 8,
  4, 4, 4,
  4, 2,
  4, 4, 4,
  2,
  4, 4, 8, 8,
  4, 4, 4,
  4, 2,
  4, 4, 4,
  2,
  4, 4, 4,
  4, 4,
  4, 1
};

const int oHolyNight_size = sizeof(oHolyNight_melody) / sizeof(int);



// 13. Rudolph the Red-Nosed Reindeer (short version)
const int rudolph_melody[] = {
  NOTE_G4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4
};

const int rudolph_tempo[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 2
};

const int rudolph_size = sizeof(rudolph_melody) / sizeof(int);

// 14. Let It Snow! Let It Snow! Let It Snow! (short version)
const int letItSnow_melody[] = {
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_G4
};

const int letItSnow_tempo[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 2
};

const int letItSnow_size = sizeof(letItSnow_melody) / sizeof(int);

// 15. All I Want for Christmas Is You (short version)
const int allIWant_melody[] = {
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_D5,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_E4, NOTE_D4, NOTE_D4
};

const int allIWant_tempo[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 2
};

const int allIWant_size = sizeof(allIWant_melody) / sizeof(int);

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
      return {weWish_melody, weWish_tempo, weWish_size, 1000};
    case SANTA_CLAUS:
      return {santaClaus_melody, santaClaus_tempo, santaClaus_size, 900};
    case SILENT_NIGHT:
      return {silentNight_melody, silentNight_tempo, silentNight_size, 1200};
    case DECK_THE_HALLS:
      return {deckHalls_melody, deckHalls_tempo, deckHalls_size, 800};
    case O_COME_ALL_YE_FAITHFUL:
      return {oComeAllYe_melody, oComeAllYe_tempo, oComeAllYe_size, 900};
    case THE_FIRST_NOEL:
      return {firstNoel_melody, firstNoel_tempo, firstNoel_size, 1000};
    case JOY_TO_THE_WORLD:
      return {joyToWorld_melody, joyToWorld_tempo, joyToWorld_size, 850};
    case HARK_THE_HERALD:
      return {harkHerald_melody, harkHerald_tempo, harkHerald_size, 950};
    case ANGELS_WE_HAVE_HEARD:
      return {angelsHeard_melody, angelsHeard_tempo, angelsHeard_size, 900};
    case GOOD_KING_WENCESLAS:
      return {goodKing_melody, goodKing_tempo, goodKing_size, 1000};
    case O_HOLY_NIGHT:
      return {oHolyNight_melody, oHolyNight_tempo, oHolyNight_size, 1100};
    case RUDOLPH:
      return {rudolph_melody, rudolph_tempo, rudolph_size, 950};
    case LET_IT_SNOW:
      return {letItSnow_melody, letItSnow_tempo, letItSnow_size, 900};
    case ALL_I_WANT:
      return {allIWant_melody, allIWant_tempo, allIWant_size, 1000};
      return {oHolyNight_melody, oHolyNight_tempo, oHolyNight_size, 1100};
    default:
      return {jingleBells_melody, jingleBells_tempo, jingleBells_size, 1000};
  }
}

#endif // CHRISTMAS_SONGS_H