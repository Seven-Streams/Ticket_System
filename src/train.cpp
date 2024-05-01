#include "../include/train.hpp"
#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
sjtu::BPT<int> train_index("train_index");
sjtu::MemoryRiver<TrainInfo, 1> train_info;
sjtu::BPT<int> station_database("station");
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
    }
    if (op == "-y") {
      if (type != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      type = ProcessTxt(command);
      CheckType(type.c_str());
    }
  }
  if ((ID == "") || (num_raw == "") || (seat_raw == "") || (stations == "") ||
      (prices == "") || (start_time == "") || (travel_time == "") ||
      (stop_time == "") || (sale_date == "") || (type == "")) {
    throw(SevenStream::exception("Invalid input."));
  }
  TrainInfo res;
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(ID, exp1);
  hash2 = sjtu::MyHash(ID, exp2);
  string exist_check = train_index.find(hash1, hash2);
  if (exist_check != "") {
    throw(SevenStream::exception("The train has been added."));
  }
  res.id_hash1 = hash1;
  res.id_hash2 = hash2;
  int num = std::stoi(num_raw);
  res.station_number = num;
  int seat_num = std::stoi(seat_raw);
  res.seat_number = seat_num;
  for (int i = 1; i < num; i++) {
    string price_raw = ProcessMalValue(prices);
    CheckPrice(price_raw.c_str());
    int price = std::stoi(price_raw);
    res.price[i] = price;
  }
  for (int i = 0; i < num; i++) {
    string station = ProcessMalValue(stations);
    CheckStation(station.c_str());
    res.station_hash1[i] = sjtu::MyHash(station, exp1);
    res.station_hash2[i] = sjtu::MyHash(station, exp2);
  }
  CheckStartTime(start_time.c_str());
  int hour = (start_time[0] - '0') * 10 + (start_time[1] - '0');
  int minute = (start_time[3] - '0') * 10 + (start_time[4] - '0');
  res.start_hour = hour;
  res.start_minute = minute;
  for (int i = 1; i < num; i++) {
    string travel_time_raw = ProcessMalValue(travel_time);
    CheckInterTime(travel_time_raw.c_str());
    int time = std::stoi(travel_time_raw);
    res.travel[i] = time;
  }
  for (int i = 1; i < (num - 1); i++) {
    string stop_time_raw = ProcessMalValue(stop_time);
    CheckInterTime(stop_time_raw.c_str());
    int time = std::stoi(stop_time_raw);
    res.stop[i] = time;
  }
  string date = ProcessMalValue(sale_date);
  CheckDate(date.c_str());
  int month = (date[0] - '0') * 10 + (date[1] - '0');
  int day = (date[3] - '0') * 10 + (date[4] - '0');
  res.sale_month = month;
  res.sale_day = day;
  date = ProcessMalValue(sale_date);
  CheckDate(date.c_str());
  month = (date[0] - '0') * 10 + (date[1] - '0');
  day = (date[3] - '0') * 10 + (date[4] - '0');
  res.des_month = month;
  res.des_day = day;

  res.type = type[0];
  int current;
  train_info.get_info(current, 1);
  train_info.write(res, ++current);
  train_info.write_info(current, 1);
  train_index.Insert(hash1, hash2, current);
  for (int i = 0; i < num; i++) {
    station_database.Insert(res.station_hash1[i], res.station_hash2[i],
                            current);
  }
  return;
}
void ReleaseTrain(string &command) {
  string op;
  op = ProcessTxt(command);
  if (op != "-i") {
    throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  CheckTrainID(id.c_str());
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  string index_raw = train_index.find(hash1, hash2);
  if (index_raw == "") {
    throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = std::stoi(index_raw);
  TrainInfo to_release;
  train_info.read(to_release, index);
  to_release.released = true;
  for (int i = 0; i < to_release.station_number; i++) {
    station_database.Insert(to_release.station_hash1[i],
                            to_release.station_hash2[i], index);
  }
  train_info.write(to_release, index);
  return;
}
void DeleteTrain(string &command) {
  string op = ProcessTxt(command);
  if(op != "-i") {
    throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  CheckTrainID(id.c_str());
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  string index_raw = train_index.find(hash1, hash2);
  if(index_raw == "") {
    throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = std::stoi(index_raw);
  TrainInfo to_delete;
  train_info.read(to_delete, index);
  if(to_delete.released) {
    throw(SevenStream::exception("The train has been released."));
  }
  train_index.Erase(hash1, hash2, index);
  return;
}