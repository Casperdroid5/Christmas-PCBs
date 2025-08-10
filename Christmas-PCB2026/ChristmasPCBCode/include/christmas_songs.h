/*  The MIT License (MIT)

    Copyright (c) 2015 Andy Tran

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

    Complete Christmas Songs Collection
*/

#ifndef CHRISTMAS_SONGS_H
#define CHRISTMAS_SONGS_H

#include "pitches.h"

#ifndef REST
#define REST 0
#endif

// Enum for all available Christmas songs
enum ChristmasSong {
  SANTA_CLAUS_IS_COMIN,
  JINGLE_BELLS,
  WE_WISH_YOU_A_MERRY_CHRISTMAS,
  SILENT_NIGHT,
  RUDOLPH_THE_RED_NOSED,
  O_CHRISTMAS_TREE,
  O_COME_ALL_YE_FAITHFUL,
  O_LITTLE_TOWN_OF_BETHLEHEM,
  THE_FIRST_NOEL,
  WE_THREE_KINGS,
  WHITE_CHRISTMAS,
  AWAY_IN_A_MANGER,
  CAROL_OF_THE_BELLS,
  DECK_THE_HALLS,
  GOD_REST_YE_MERRY_GENTLEMEN,
  GO_TELL_IT_ON_THE_MOUNTAIN,
  HARK_THE_HERALD_ANGELS_SING,
  JOY_TO_THE_WORLD,
  NUM_CHRISTMAS_SONGS
};

// Song names for reference
const char* songNames[] = {
  "Santa Claus Is Coming to Town",
  "Jingle Bells",
  "We Wish You a Merry Christmas",
  "Silent Night",
  "Rudolph the Red-Nosed Reindeer",
  "O Christmas Tree",
  "O Come All Ye Faithful",
  "O Little Town of Bethlehem",
  "The First Noel",
  "We Three Kings",
  "White Christmas",
  "Away in a Manger",
  "Carol of the Bells",
  "Deck the Halls",
  "God Rest Ye Merry Gentlemen",
  "Go Tell It on the Mountain",
  "Hark the Herald Angels Sing",
  "Joy to the World"
};

// Structure to hold song data
struct Song {
  const int16_t* melody;
  int size;
  int baseTempo;
  const char* name;
};

// 1. Santa Claus Is Coming to Town
const int16_t santaClausIsComin[] PROGMEM {
  NOTE_G4,8,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
  NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
  NOTE_D4,4, NOTE_F4,2, NOTE_B3,4,
  NOTE_C4,-2, REST,4,
  NOTE_G4,8,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
  NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
  NOTE_D4,4, NOTE_F4,2, NOTE_D5,4,
  NOTE_C5,1
};

// 2. Jingle Bells (Classic)
const int16_t jingleBells[] PROGMEM {
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
  NOTE_E5,4, NOTE_G5,4, NOTE_C5,4, NOTE_D5,4,
  NOTE_E5,1,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,8, NOTE_E5,8,
  NOTE_E5,4, NOTE_D5,4, NOTE_D5,4, NOTE_E5,4,
  NOTE_D5,2, NOTE_G5,2
};

// 3. We Wish You a Merry Christmas
const int16_t weWishYou[] PROGMEM {
  NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,

  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2
};

// 4. Silent Night
const int16_t silentNight[] PROGMEM {
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_D5,2, NOTE_D5,4,
  NOTE_B4,-2,
  NOTE_C5,2, NOTE_C5,4,
  NOTE_G4,-2,
  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_D5,2, NOTE_D5,4,
  NOTE_F5,-4, NOTE_D5,8, NOTE_B4,4,
  NOTE_C5,-2,
  NOTE_E5,-2,
  NOTE_C5,4, NOTE_G4,4, NOTE_E4,4,
  NOTE_G4,-4, NOTE_F4,8, NOTE_D4,4,
  NOTE_C4,-2
};

// 5. Rudolph the Red-Nosed Reindeer
const int16_t rudolfTheRedNosed[] PROGMEM {
  NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
  NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_A4,4, NOTE_E4,1,
  NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
  NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1,
  NOTE_A4,4, NOTE_A4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,4, NOTE_E4,4, NOTE_G4,2, NOTE_F4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,4, NOTE_E4,1,
  NOTE_D4,4, NOTE_E4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,4, NOTE_B4,4, NOTE_B4,2, NOTE_C5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,8, NOTE_D4,-2,
  NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
  NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1
};

// 6. O Christmas Tree
const int16_t oChristmasTree[] PROGMEM {
  NOTE_D4,4, NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,4, NOTE_D4,2, REST,16,
  NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,-4, REST,4, NOTE_D5,8,
  NOTE_D5,8, NOTE_B4,8, NOTE_E5,-4, NOTE_D5,8, NOTE_D5,8, NOTE_C5,8, NOTE_C5,-4, NOTE_C5,8, NOTE_C5,8, NOTE_A4,8, NOTE_D5,-4, NOTE_C5,8, NOTE_C5,8, NOTE_B4,8, NOTE_B4,4, NOTE_D4,4,
  NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,2
};

// 7. O Come All Ye Faithful
const int16_t oComeAllYe[] PROGMEM {
  NOTE_G4,4, NOTE_G4,2, NOTE_D4,4, NOTE_G4,4, NOTE_A4,2, NOTE_D4,2, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_C5,4, NOTE_B4,2,
  NOTE_A4,4, NOTE_G4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,4, NOTE_FS4,2, NOTE_E4,-4, NOTE_D4,8, NOTE_D4,1,
  NOTE_D5,2, NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_B4,2, NOTE_A4,4, NOTE_B4,4, NOTE_G4,4, NOTE_A4,4, NOTE_FS4,-4, NOTE_E4,8, NOTE_D4,4,
  NOTE_G4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_G4,2, NOTE_D4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_C5,4, NOTE_B4,2, NOTE_A4,4,
  NOTE_B4,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,2, NOTE_G4,4, NOTE_C5,4, NOTE_B4,2, NOTE_A4,-4, NOTE_G4,8, NOTE_G4,-2
};

// 8. O Little Town of Bethlehem
const int16_t oLittleTown[] PROGMEM {
  NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4, NOTE_D4,4, NOTE_B4,-2,
  NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_E5,4, NOTE_D5,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2,
  NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,2, NOTE_FS4,4, NOTE_FS4,4, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-2,
  NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_E5,4, NOTE_D5,4, NOTE_G4,4, NOTE_B4,-4, NOTE_A4,8, NOTE_G4,-2
};

// 9. The First Noel
const int16_t theFirstNoel[] PROGMEM {
  NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4,
  NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4,
  NOTE_FS4,2, NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4,
  NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4,
  NOTE_FS4,2, NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_D5,8, NOTE_CS5,8, NOTE_B4,2, NOTE_B4,4,
  NOTE_A4,-2, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,-2
};

// 10. We Three Kings
const int16_t weThreeKings[] PROGMEM {
  NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
  NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
  NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,-2, REST,4,
  NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
  NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
  NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,1
};

// 11. White Christmas
const int16_t whiteChristmas[] PROGMEM {
  NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, NOTE_D5,-2, REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4, NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
  NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2, NOTE_D5,4, NOTE_G4,2, NOTE_G4,2, NOTE_G4,4, NOTE_D5,2, NOTE_C5,4, NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_A4,-1,
  NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, NOTE_D5,-2, REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4, NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
  NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2, NOTE_D5,4, NOTE_G5,1, REST,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,2, NOTE_B4,2, NOTE_E5,4, NOTE_E5,4, NOTE_FS4,4, NOTE_FS4,4, NOTE_G4,-1
};

// 12. Away in a Manger
const int16_t awayInAManger[] PROGMEM {
  NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2,
  NOTE_G3,4, NOTE_G3,-4, NOTE_A3,8, NOTE_G3,4, NOTE_G3,4, NOTE_D4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_C4,4, NOTE_E4,2,
  NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2,
  NOTE_G3,4, NOTE_F4,-4, NOTE_E4,8, NOTE_D4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,-2
};

// 13. Carol of the Bells
const int16_t carolOfTheBells[] PROGMEM {
  NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4,
  NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_G5,4, NOTE_G5,8, NOTE_G5,8, NOTE_F5,8, NOTE_DS5,8, NOTE_D5,4, NOTE_D5,8, NOTE_D5,8, NOTE_C5,8, NOTE_AS4,8,
  NOTE_C5,4, NOTE_C5,8, NOTE_C5,8, NOTE_D5,8, NOTE_C5,8, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,8, NOTE_AS4,8, NOTE_C5,8, NOTE_D5,8, NOTE_C5,4, NOTE_AS4,4, NOTE_D5,8, NOTE_D5,8, NOTE_FS5,8, NOTE_G5,8, NOTE_A5,8, NOTE_AS5,8,
  NOTE_C6,8, NOTE_D6,8, NOTE_C6,4, NOTE_AS5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4,
  NOTE_AS5,-4, NOTE_A5,-8, NOTE_AS5,-8, NOTE_G5,2
};

// 14. Deck the Halls
const int16_t deckTheHalls[] PROGMEM {
  NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_D4,8, NOTE_E4,-4, NOTE_D4,8, NOTE_C4,4, NOTE_B3,4, NOTE_C4,2, 
  NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_D4,8, NOTE_E4,-4, NOTE_D4,8, NOTE_C4,4, NOTE_B3,4, NOTE_C4,2,
  NOTE_D4,-4, NOTE_E4,8, NOTE_F4,4, NOTE_D4,4, NOTE_E4,-4, NOTE_F4,8, NOTE_G4,4, NOTE_D4,4, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,4, NOTE_A3,8, NOTE_B3,8, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2, 
  NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,8, NOTE_A3,8, NOTE_A3,8, NOTE_A3,8, NOTE_G3,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,2
};

// 15. God Rest Ye Merry Gentlemen
const int16_t godRestYe[] PROGMEM {
  NOTE_A3,4, NOTE_A3,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_A3,4,
  NOTE_A3,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_E4,4, NOTE_G4,4, NOTE_D4,4, NOTE_E4,4, NOTE_G4,4,
  NOTE_G4,4, NOTE_A4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,2, NOTE_C4,4, NOTE_D4,4, NOTE_E4,2, NOTE_G4,4, NOTE_E4,4,
  NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,2, NOTE_C4,8, NOTE_B3,8, NOTE_A3,4, NOTE_D4,2, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_F4,4, NOTE_G4,4, NOTE_A4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,1
};

// 16. Go Tell It on the Mountain
const int16_t goTellItOnTheMountain[] PROGMEM {
  NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,2, NOTE_D4,8, NOTE_D4,4, NOTE_D4,8, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,4,
  NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,4, NOTE_F4,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,8, NOTE_C4,8, NOTE_D4,4, NOTE_C4,-2, NOTE_C4,4,
  NOTE_E4,4, NOTE_A4,4, NOTE_A4,-4, NOTE_A4,8, NOTE_A4,4, NOTE_E4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_C4,4,
  NOTE_E4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4, NOTE_E4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,4, NOTE_D4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,2, NOTE_F4,2,
  NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,2, NOTE_D4,8, NOTE_D4,4, NOTE_D4,8, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,4,
  NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,4, NOTE_F4,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,8, NOTE_C4,8, NOTE_D4,4, NOTE_C4,-2
};

// 17. Hark the Herald Angels Sing
const int16_t harkTheHerald[] PROGMEM {
  NOTE_D4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_G4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_D5,-4, NOTE_C5,8, NOTE_B4,4, NOTE_A4,4, NOTE_B4,2, NOTE_D4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_G4,4,
  NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_A4,4, NOTE_A4,-4, NOTE_F4,8, NOTE_F4,4, NOTE_E4,8, NOTE_D4,8, NOTE_D4,2, 

  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4, NOTE_G4,4, NOTE_C5,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_D5,4, NOTE_G4,4, NOTE_C5,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,-4, NOTE_G4,8, NOTE_G4,4, NOTE_A4,4, NOTE_B4,2, 

  NOTE_E5,-4, NOTE_E5,8, NOTE_E5,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,-4, NOTE_G4,8, NOTE_G4,4, NOTE_A4,4, NOTE_G4,2
};

// 18. Joy to the World
const int16_t joyToTheWorld[] PROGMEM {
  NOTE_F5,2, NOTE_E5,-4, NOTE_D5,8, NOTE_C5,-2, NOTE_AS4,4, NOTE_A4,2, NOTE_G4,2, NOTE_F4,-2,
  NOTE_C5,4, NOTE_D5,-2, NOTE_D5,4, NOTE_E5,-2, NOTE_E5,4, NOTE_F5,1,
  NOTE_F5,4, NOTE_F5,4, NOTE_E5,4, NOTE_D5,4, NOTE_C5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_F5,4, NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_C5,-2,
  NOTE_AS4,8, NOTE_A4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4, NOTE_G4,8, NOTE_A4,8, NOTE_AS4,-2,
  NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F5,2, NOTE_D5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_C5,4, NOTE_E4,2, NOTE_D4,2, NOTE_C4,1
};

// Default tempos for each song
const int defaultTempos[] = {
  137,  // Santa Claus Is Coming to Town
  180,  // Jingle Bells (updated from repository)
  160,  // We Wish You a Merry Christmas
  130,  // Silent Night
  150,  // Rudolph the Red-Nosed Reindeer
  115,  // O Christmas Tree
  140,  // O Come All Ye Faithful
  125,  // O Little Town of Bethlehem
  130,  // The First Noel
  144,  // We Three Kings
  155,  // White Christmas
  125,  // Away in a Manger
  135,  // Carol of the Bells
  150,  // Deck the Halls
  170,  // God Rest Ye Merry Gentlemen
  140,  // Go Tell It on the Mountain
  140,  // Hark the Herald Angels Sing
  160   // Joy to the World
};

// Array of all songs
const Song songs[] = {
  {santaClausIsComin, sizeof(santaClausIsComin)/sizeof(int16_t)/2, 137, "Santa Claus Is Coming to Town"},
  {jingleBells, sizeof(jingleBells)/sizeof(int16_t)/2, 180, "Jingle Bells"},
  {weWishYou, sizeof(weWishYou)/sizeof(int16_t)/2, 160, "We Wish You a Merry Christmas"},
  {silentNight, sizeof(silentNight)/sizeof(int16_t)/2, 130, "Silent Night"},
  {rudolfTheRedNosed, sizeof(rudolfTheRedNosed)/sizeof(int16_t)/2, 150, "Rudolph the Red-Nosed Reindeer"},
  {oChristmasTree, sizeof(oChristmasTree)/sizeof(int16_t)/2, 115, "O Christmas Tree"},
  {oComeAllYe, sizeof(oComeAllYe)/sizeof(int16_t)/2, 140, "O Come All Ye Faithful"},
  {oLittleTown, sizeof(oLittleTown)/sizeof(int16_t)/2, 125, "O Little Town of Bethlehem"},
  {theFirstNoel, sizeof(theFirstNoel)/sizeof(int16_t)/2, 130, "The First Noel"},
  {weThreeKings, sizeof(weThreeKings)/sizeof(int16_t)/2, 144, "We Three Kings"},
  {whiteChristmas, sizeof(whiteChristmas)/sizeof(int16_t)/2, 155, "White Christmas"},
  {awayInAManger, sizeof(awayInAManger)/sizeof(int16_t)/2, 125, "Away in a Manger"},
  {carolOfTheBells, sizeof(carolOfTheBells)/sizeof(int16_t)/2, 135, "Carol of the Bells"},
  {deckTheHalls, sizeof(deckTheHalls)/sizeof(int16_t)/2, 150, "Deck the Halls"},
  {godRestYe, sizeof(godRestYe)/sizeof(int16_t)/2, 170, "God Rest Ye Merry Gentlemen"},
  {goTellItOnTheMountain, sizeof(goTellItOnTheMountain)/sizeof(int16_t)/2, 140, "Go Tell It on the Mountain"},
  {harkTheHerald, sizeof(harkTheHerald)/sizeof(int16_t)/2, 140, "Hark the Herald Angels Sing"},
  {joyToTheWorld, sizeof(joyToTheWorld)/sizeof(int16_t)/2, 160, "Joy to the World"}
};

// Function to get song data
inline Song getSongData(ChristmasSong song) {
  return songs[song];
}

// Function to get song by index
inline Song getSongByIndex(int index) {
  if (index >= 0 && index < NUM_CHRISTMAS_SONGS) {
    return songs[index];
  }
  return songs[0]; // Return first song as default
}

// Function to get total number of songs
inline int getTotalSongs() {
  return NUM_CHRISTMAS_SONGS;
}

#endif // CHRISTMAS_SONGS_H