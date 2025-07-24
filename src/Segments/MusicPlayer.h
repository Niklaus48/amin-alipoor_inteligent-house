#pragma once
#include "IComponent.h"
#include "../Observer_Pattern/IObserver.h"
#include "HouseState.h"

namespace segments {

    class Note {
      public:
        Note(int tempoCount, int freq);
        float PlayNote(int buzzerPin);
      private:
        int tempoCount;
        int freq;//if set to 0 means its a silence

    };


  class MusicPlayer : public IComponent, public IObserver<HouseState> {
  public:
    MusicPlayer(int buzzerPin);
    void PlayMusic();
    void SetMusic(Note* notes);
    void Init() override;
    void Update() override;
    void onNotify(const HouseState& state) override;

  private:
    Note* notes;
    int noteCount;
    int relatedBuzzerPin;
    int current;
    unsigned long elapsedTime;

  };
}
