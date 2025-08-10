#include <Arduino.h>
#include "christmas_songs.h"

#define BUZZERPIN 10

// Function declarations
void playMusic(const int16_t melody[], uint16_t numNotes, uint16_t songTempo);
void playSong(ChristmasSong song);
void playRandomSong();

// Global variables
int currentSongIndex = 0;
unsigned long lastPlayTime = 0;
const unsigned long SONG_INTERVAL = 3000; // 3 seconds between songs

void setup() {
  pinMode(BUZZERPIN, OUTPUT);
  Serial.begin(115200);
  
  Serial.println("Christmas Song Player Ready!");
  Serial.println("Available songs:");
  for (int i = 0; i < NUM_CHRISTMAS_SONGS; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(songNames[i]);
  }
  Serial.println("\nPlaying all songs in sequence...");
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastPlayTime >= SONG_INTERVAL) {
    // Play current song
    Song currentSong = getSongByIndex(currentSongIndex);
    
    Serial.print("Now playing: ");
    Serial.println(currentSong.name);
    
    playMusic(currentSong.melody, currentSong.size, currentSong.baseTempo);
    
    // Move to next song
    currentSongIndex++;
    if (currentSongIndex >= NUM_CHRISTMAS_SONGS) {
      currentSongIndex = 0;
      Serial.println("\n--- Starting playlist over ---\n");
    }
    
    lastPlayTime = currentTime;
  }
}

void playMusic(const int16_t melody[], uint16_t numNotes, uint16_t songTempo) {
  uint16_t wholeNote = (60000 * 4) / songTempo;
  uint16_t noteDuration;
  int8_t noteType;
  
  for (uint16_t i = 0; i < numNotes * 2; i += 2) {
    noteType = pgm_read_word(&melody[i + 1]);
    if (noteType > 0) {
      noteDuration = wholeNote / noteType;
    }
    else {
      noteDuration = wholeNote / abs(noteType) * 1.5;
    }
    tone(BUZZERPIN, pgm_read_word(&melody[i]), noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void playSong(ChristmasSong song) {
  Song songData = getSongData(song);
  Serial.print("Playing: ");
  Serial.println(songData.name);
  playMusic(songData.melody, songData.size, songData.baseTempo);
}

void playRandomSong() {
  int randomIndex = random(0, NUM_CHRISTMAS_SONGS);
  Song randomSong = getSongByIndex(randomIndex);
  Serial.print("Random song: ");
  Serial.println(randomSong.name);
  playMusic(randomSong.melody, randomSong.size, randomSong.baseTempo);
}

// Alternative loop for playing specific songs
void playSpecificSongs() {
  // Example: Play only a few favorite songs
  ChristmasSong favorites[] = {
    SANTA_CLAUS_IS_COMIN,
    JINGLE_BELLS,
    SILENT_NIGHT,
    WE_WISH_YOU_A_MERRY_CHRISTMAS
  };
  
  for (int i = 0; i < 4; i++) {
    playSong(favorites[i]);
    delay(2000);
  }
}