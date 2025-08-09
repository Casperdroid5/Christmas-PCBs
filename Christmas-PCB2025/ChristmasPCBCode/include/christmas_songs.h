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

    Christmas Songs Data
*/

#ifndef CHRISTMAS_SONGS_H
#define CHRISTMAS_SONGS_H

#include "pitches.h"

#ifndef REST
#define REST 0
#endif

enum ChristmasSong {
  JINGLE_BELLS,
  WE_WISH,
  RUDOLPH,
  LET_IT_SNOW,
  ALL_I_WANT,
  SILENT_NIGHT,
  NUM_CHRISTMAS_SONGS
};

const char* songNames[] = {
  "Jingle Bells",
  "We Wish You a Merry Christmas",
  "Rudolph the Red-Nosed Reindeer",
  "Let It Snow! Let It Snow! Let It Snow!",
  "All I Want for Christmas Is You",
  "Silent Night"
};

// Structure to hold song data
struct Song {
  const int* melody;
  const int* tempo;
  int size;
  int baseTempo;
  const char* name;
};

// 1. Jingle Bells (Classic version)
const int jingleBells_melody[] = {
  659, 659, 659, 659, 659, 659,
  659, 784, 523, 587, 659,
  698, 698, 698, 698, 698, 659, 659, 659,
  659, 587, 587, 659, 587, 784
};

const int jingleBells_tempo[] = {
  8, 8, 4, 8, 8, 4,
  8, 8, 8, 8, 2,
  8, 8, 8, 8, 8, 8, 16, 16,
  8, 8, 8, 8, 4, 4
};

// 2. We Wish You a Merry Christmas (Classic version)
const int weWish_melody[] = {
  392, 392, 440, 392, 349, 330,
  392, 392, 440, 392, 349, 330,
  294, 294, 330, 294, 262,
  392, 392, 440, 392, 349, 330,
  392, 392, 440, 392, 349, 330,
  294, 294, 330, 294, 262
};

const int weWish_tempo[] = {
  4, 4, 8, 8, 4, 2,
  4, 4, 8, 8, 4, 2,
  4, 4, 8, 8, 2,
  4, 4, 8, 8, 4, 2,
  4, 4, 8, 8, 4, 2,
  4, 4, 8, 8, 2
};

// 3. Rudolph the Red-Nosed Reindeer (Classic Version)
const int rudolph_melody[] = {
  262, 262, 262, 294, 330, 262, 330, 294,
  262, 262, 262, 294, 330, 262,
  247, 247, 247, 262, 220,
  196,
  
  262, 262, 262, 294, 330, 262, 330, 294,
  262, 262, 262, 294, 330, 262,
  247, 247, 247, 262, 220,
  196,
  
  330, 330, 330, 349, 392,
  330, 294, 262, 294, 330,
  262, 220, 196, 220, 247,
  262,
  
  262, 262, 262, 294, 330, 262,
  294, 294, 294, 330, 294, 262, 220,
  784, 330, 294, 262, 220,
  196
};

const int rudolph_tempo[] = {
  4, 4, 4, 8, 8, 4, 8, 8,
  4, 4, 4, 8, 8, 2, 
  8, 8, 8, 8, 2,
  2,
  
  4, 4, 4, 8, 8, 4, 8, 8,
  4, 4, 4, 8, 8, 2,
  8, 8, 8, 8, 2,
  2,
  
  4, 4, 4, 8, 8,
  4, 8, 8, 8, 8,
  4, 8, 8, 8, 8,
  2,
  
  4, 4, 4, 8, 8, 4,
  8, 8, 8, 8, 8, 8, 8,
  4, 8, 8, 4, 2,
  1
};

// 4. Let It Snow (Classic version)
const int letItSnow_melody[] = {
  523, 587, 659, 698, 784, 880, 784, 698, 659, 587,
  523, 587, 659, 698, 784, 880, 784, 698, 659, 587,
  523, 587, 659, 698, 784, 880, 988, 1047,
  880, 784, 698, 659, 587, 659, 698,
  659, 587, 523, 494, 523
};

const int letItSnow_tempo[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2
};

// 5. All I Want for Christmas Is You (Main melody)
const int allIWant_melody[] = {
  659, 659, 659, 784, 698, 659, 587,
  523, 523, 587, 523, 494,
  440, 440, 494, 440, 392,
  349, 392, 440, 494, 523, 587, 659,
  698, 659, 587, 523, 494,
  523, 587, 659, 698, 784, 880,
  784, 698, 659, 587, 523
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

// 6. Silent Night (Classic version)
const int silentNight_melody[] = {
  392, 440, 392, 330,
  392, 440, 392, 330,
  587, 587, 494,
  523, 523, 392,
  440, 440, 523, 494, 440,
  392, 440, 392, 330,
  440, 440, 523, 494, 440,
  392, 440, 392, 330,
  587, 587, 698, 587, 494,
  523, 659,
  523, 392, 330,
  392, 349, 294,
  262
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
  2
};

// Array of all songs
const Song songs[] = {
  {jingleBells_melody, jingleBells_tempo, sizeof(jingleBells_melody)/sizeof(int), 1000, "Jingle Bells"},
  {weWish_melody, weWish_tempo, sizeof(weWish_melody)/sizeof(int), 1200, "We Wish You a Merry Christmas"},
  {rudolph_melody, rudolph_tempo, sizeof(rudolph_melody)/sizeof(int), 1200, "Rudolph the Red-Nosed Reindeer"},
  {letItSnow_melody, letItSnow_tempo, sizeof(letItSnow_melody)/sizeof(int), 1100, "Let It Snow"},
  {allIWant_melody, allIWant_tempo, sizeof(allIWant_melody)/sizeof(int), 1000, "All I Want for Christmas Is You"},
  {silentNight_melody, silentNight_tempo, sizeof(silentNight_melody)/sizeof(int), 1400, "Silent Night"}
};

// Function to get song data
inline Song getSongData(ChristmasSong song) {
  return songs[song];
}

#endif // CHRISTMAS_SONGS_H