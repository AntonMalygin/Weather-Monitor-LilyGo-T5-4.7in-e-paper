// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "WeatherData.h"

WeatherRoot_t         WeatherRoot;
WeatherCurrent_t      WeatherCurrent;
WeatherHourly_t       WeatherHourly;
WeatherMinutely_t     WeatherMinutely;
WeatherDaily_t        WeatherDaily;
WeatherAlert_t        WeatherAlert;
WeatherHistory_t      WeatherHistory;



float getMean(float * values, int start, int count) {
  float result = 0;
  for (int i = start; i < start + count; ++i) {
    result += values[i];
  }
  result = result / count;
  return result;
}

float getMean(int * values, int start, int count) {
  float result = 0;
  for (int i = start; i < start + count; ++i) {
    result += values[i];
  }
  result = result / count;
  return result;
}

float getMin(int * values, int start, int count) {
  float v = values[start];
  float min = v;
  for (int i = start + 1; i < start + count; ++i) {
    v = values[i];
    if (v < min)
      min = v;
  }
  return min;
}

float getMin(float * values, int start, int count) {
  float v = values[start];
  float min = v;
  for (int i = start + 1; i < start + count; ++i) {
    v = values[i];
    if (v < min)
      min = v;
  }
  return min;
}

float getMax(float * values, int start, int count) {
  float v = values[start];
  float max = v;
  for (int i = start + 1; i < start + count; ++i) {
    v = values[i];
    if (v > max)
      max = v;
  }
  return max;
}

float getSum(float * values, int start, int count) {
  float result = 0;
  for (int i = start; i < start + count; ++i) {
    result += values[i];
  }
  return result;
}

float getDeltaTemperature() {
  // Разность средней температуры за предыдущие и прогнозные 24 часа
  const int count = 24;
  if (WeatherHistory.Count < count) {
    return 0;
  }
  int start = WeatherHistory.Count - count;
  float meanHistory = getMean(WeatherHistory.Temperature, start, count);
  float meanFuture  = getMean(WeatherHourly.Temperature,  0, count);
  float delta = meanFuture - meanHistory;
  DEBUG("DeltaTemperature: future=" + String(meanFuture) + " history=" + String(meanHistory) + " delta=" + String(delta));
  return delta;
}

float getDeltaPressure(int hour) {
  // Разность текщего атмосферного давления по сравнению со среднем за предыдущеи 24 часа
  const int count = hour;
  if (WeatherHistory.Count < count) {
    return 0;
  }
  int start = WeatherHistory.Count - count;
  float mean = getMean(WeatherHistory.Pressure, start, count);
  float delta = WeatherCurrent.Pressure - mean;
  DEBUG("DeltaPressure: mean=" + String(mean) + " delta=" + String(delta));
  return delta;
}

// Выбираем из массива наиболее часто встречающуюся погоду
int selectWeatherItem(WeatherItem_t * Weather, int start, int count) {
  // DEBUG("selectWeatherItem: start=" + String(start) + " count=" + String(count));

  typedef struct {
  int  WeatherId;
  int  Count;
  int  Index;
  } Item_t;

  // Item_t items[6];
  Item_t * items = new Item_t [count];

  // Очистка массива
  for (int i = 0; i < count; ++i) {
    items[i].WeatherId  = -1;
    items[i].Index      = -1;
    items[i].Count      = 0;
  };

  // Подсчет количества повторов каждого идентификатора погоды
  for (int n = 0; n < count; ++n){
    int index = start + n;
    int WeatherId = Weather[index].Id;
    for (int i = 0; i < count; ++i){
      if ((items[i].WeatherId == WeatherId) || (items[i].WeatherId == -1)) {
        items[i].WeatherId = WeatherId;
        items[i].Index     = index;
        items[i].Count++;
        // DEBUG("Id: " + String(WeatherId) +  " Index: " + String(index) + " Count: " + String(items[i].Count));
        break;
      };
    };
  };

  int index = 0;
  int max   = 0;
  for (int i = 0; i < count; ++i){
    int cnt = items[i].Count;
    if (cnt == 0)
      break;
    if( cnt > max) {
      max = cnt;
      index = items[i].Index;
    }
    //DEBUG("#" + String(i) + " Id: " + String(items[i].Id) + " Count: " + String(cnt));
  };
  // DEBUG("Index: " + String(index));
  delete [] items;
  return index;

}
