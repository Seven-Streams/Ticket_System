#include "../include/train.hpp"
#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
sjtu::BPT<int> train_index("train_index");
sjtu::MemoryRiver<TrainInfo, 1> train_info;
void AddTrain(std::string &command) {
  string ID, num_raw, seat_raw, stations, prices, start_time, travel_time,
      stop_time, sale_date, type;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-i") {
      if (ID != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      ID = ProcessTxt(command);
      CheckTrainID(ID.c_str());
    }
    if (op == "-n") {
      if (num_raw != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      num_raw = ProcessTxt(command);
      CheckStatNum(num_raw.c_str());
    }
    if (op == "-m") {
      if (seat_raw != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      seat_raw = ProcessTxt(command);
      CheckSeatNum(seat_raw.c_str());
    }
    if (op == "-s") {
      if (stations != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      stations = ProcessTxt(command);
    }
    if (op == "-p") {
      if (prices != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      prices = ProcessTxt(command);
    }
    if (op == "-x") {
      if (start_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      start_time = ProcessTxt(command);
      CheckStartTime(start_time.c_str());
    }
    if (op == "-t") {
      if (travel_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      travel_time = ProcessTxt(command);
    }
    if (op == "-o") {
      if (stop_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      stop_time = ProcessTxt(command);
    }
    if (op == "-d") {
      if (sale_date != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      sale_date = ProcessTxt(command);
      CheckDate(sale_date.c_str());
    }
    if (op == "-y") {
      if (type != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      type = ProcessTxt(command);
      CheckType(type.c_str());
    }
  }
  if((ID == "") || (num_raw == "") || (seat_raw == "") || (stations == "") || (prices == "") || (start_time == "") || (travel_time == "") || (stop_time == "") || (sale_date == "") || (type == "")) {
    throw(SevenStream::exception("Invalid input."));
  }
  TrainInfo res;
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(ID, exp1);
  hash2 = sjtu::MyHash(ID, exp2);
  int exist_check = train_index.find(hash1, hash2);
  if (exist_check != 0) {
    throw(SevenStream::exception("The train has been added."));
  }
  res.id_hash1 = hash1;
  res.id_hash2 = hash2;
  int num = std::stoi(num_raw);
  res.station_number = num;
  int seat_num = std::stoi(seat_raw);
  res.seat_number = seat_num;
  for(int i = 1; i < num; i++) {
    string price_raw = ProcessMalValue(prices);
    CheckPrice(price_raw.c_str());
    int price = std::stoi(price_raw);
    res.price[i] = price;
  }
  CheckStartTime(start_time.c_str());
  int hour = (start_time[0] - '0') * 10 + (start_time[1] - '0');
  int minute = (start_time[3] - '0') * 10 + (start_time[4] - '0');
  res.start_hour = hour;
  res.start_minute = minute;
  for(int i = 1; i < num; i++) {
    string travel_time_raw = ProcessMalValue(travel_time);
    CheckInterTime(travel_time_raw.c_str());
    int time = std::stoi(travel_time_raw);
    res.travel[i] = time;
  }
  for(int i = 1; i < (num - 1); i++) {
    string stop_time_raw = ProcessMalValue(stop_time);
    CheckInterTime(stop_time_raw.c_str());
    int time = std::stoi(stop_time_raw);
    res.stop[i] = time;
  }
  int month = (sale_date[0] - '0') * 10 + (sale_date[1] - '0');
  int day = (sale_date[3] - '0') * 10 + (sale_date[4] - '0');
  res.sale_month = month;
  res.sale_day = day;
  res.type = type[0];
  int current;
  train_info.get_info(current, 1);
  train_info.write(res, ++current);
  train_info.write_info(current, 1);
  train_index.Insert(hash1, hash2, current);
  return;
}