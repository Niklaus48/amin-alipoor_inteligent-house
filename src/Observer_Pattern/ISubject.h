#pragma once
#include <vector>
#include "IObserver.h"

template<typename T>
class ISubject {
private:
  std::vector<IObserver<T>*> observers;

public:
  virtual ~ISubject() = default;

  virtual void AddObserver(IObserver<T>* observer){
    observers.push_back(observer);
  }
  virtual void NotifyObservers(const T& state){
    for (auto* observer : observers) {
      observer->onNotify(state);
    }
  }
};

