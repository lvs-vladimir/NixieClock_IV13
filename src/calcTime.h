void calculateTime() {
  second++;
  if (second > 59) {
    second = 0;
    minute++;
    minsCount++;
    cryptoCount++;
  }
  //Каждые 23 часа синхронизируем врем с сервером UDP
  if (hourCount >= 23) {
    hourCount = 0;
    TimeUpdate();
  }
  if (minute > 59) {
    minute = 0;
    hourCount++;
    hour++;
    if (hour > 23) hour = 0;
  }
}