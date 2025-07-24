#pragma once

template<typename T>
class IObserver {
public:
  virtual ~IObserver() = default;
  virtual void onNotify(const T& state) = 0;
};


