#include <Arduino.h>
#include "MusicPlayer.h"

namespace segments{

    Note::Note(int tempoCount, int freq){
        this->freq = freq;
        this->tempoCount = tempoCount;
    }

    float Note::PlayNote(int buzzerPin){
        if (freq == 0) {
            noTone(buzzerPin);
        }
        else {
            tone(buzzerPin, freq, tempoCount * 450);
        }

        return tempoCount * 500;
    }

    MusicPlayer::MusicPlayer(int buzzerPin){
        relatedBuzzerPin = buzzerPin;
    }

    void MusicPlayer::PlayMusic() {
        unsigned long time = millis();
        if (time >= elapsedTime) {
            float duration = notes[current].PlayNote(relatedBuzzerPin);
            current++;
            if (current >= noteCount) current = 0;
            //delay(duration);
            elapsedTime = time + duration;
        }
    }

    void MusicPlayer::SetMusic(Note* notes){
        this->notes = notes;
        current = 0;
        elapsedTime = 0;
    }

    void MusicPlayer::Init(){

    }

    void MusicPlayer::Update(){
        
    }

    void MusicPlayer::onNotify(const HouseState& state){
        
    }
}