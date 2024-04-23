#include "../include/valid.hpp"
void CheckUsername(const char *str) {
  int len = strlen(str);
  if (len > 20 || (!len)) {
    throw(SevenStream::exception("InvalidUsername"));
  }
  if ((str[0] < 'a') || str[0] > 'z') {
    if ((str[0] < 'A') || str[0] > 'Z') {
      throw(SevenStream::exception("InvalidUsername"));
    }
  }
  for (int i = 0; i < len; ++i) {
    if (str[i] < '0' || str[i] > '9') {
      if (str[i] < 'a' || str[i] > 'z') {
        if (str[i] < 'A' || str[i] > 'Z') {
          if (str[i] != '_') {
            throw(SevenStream::exception("InvalidUsername"));
          }
        }
      }
    }
  }
  return;
}
void CheckPassword(const char *str) {
  int len = strlen(str);
  if ((!len) || (len > 30)) {
    throw(SevenStream::exception("InvalidPassword"));
  }
  for (int i = 0; i < len; ++i) {
    if (str[i] <= 32 || str[i] > 126) {
      throw(SevenStream::exception("InvalidPassword"));
    }
  }
  return;
}
void Checkname(const char *str) {
  int len = strlen(str);
  int total = 0;
  for (int i = 0; i < len; i++, total++) {
    if (str[i] > 0) {
      long long byte1 = static_cast<unsigned char>(str[i]);
      long long trans = byte1;
      int how_many = 0;
      for (int i = 8; i; i--) {
        if (byte1 & (1 << (i - 1))) {
          how_many++;
        } else {
          break;
        }
      }
      if (how_many == 0) {
        how_many = 1;
        trans = byte1;
      }
      if (how_many == 2) {
        if (i + 1 >= len) {
          throw(SevenStream::exception("InvalidName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 192;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
      }
      if (how_many == 3) {
        if (i + 2 >= len) {
          throw(SevenStream::exception("InvalidName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 224;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
        trans <<= 6;
        long long byte3 = static_cast<unsigned char>(str[i + 2]);
        trans -= 128;
        trans += byte3;
      }
      if (how_many == 4) {
        if (i + 3 >= len) {
          throw(SevenStream::exception("InvalidName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 240;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
        trans <<= 6;
        long long byte3 = static_cast<unsigned char>(str[i + 2]);
        trans -= 128;
        trans <<= 6;
        long long byte4 = static_cast<unsigned char>(str[i + 3]);
        trans += byte4;
        trans -= 128;
      }
      if (trans == 0x0087 || trans == 0x2014 || trans == 0xFF08 ||
          trans == 0xFF09 || trans == 0xFF1A) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x4E00 && trans <= 0x9FFF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x3400 && trans <= 0x4DBF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x20000 && trans <= 0x2A6DF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2A700 && trans <= 0x2B73F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2B740 && trans <= 0x2B81F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2B820 && trans <= 0x2CEAF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2CEB0 && trans <= 0x2EBEF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x30000 && trans <= 0x3134F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x31350 && trans <= 0x323AF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0xF900 && trans <= 0xFAFF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2F800 && trans <= 0x2FA1F) {
        i += (how_many - 1);
        continue;
      }
      throw(SevenStream::exception("InvalidName"));
    } else {
      throw(SevenStream::exception("InvalidName"));
    }
  }
  if ((total > 5) || (total < 2)) {
    throw(SevenStream::exception("InvalidName"));
  }
  return;
}
void CheckMail(const char *str) {
  int len = strlen(str);
  if ((!len) || (len > 30)) {
    throw(SevenStream::exception("InvalidMail"));
  }
  for (int i = 0; i < len; ++i) {
    if (str[i] < '0' || str[i] > '9') {
      if (str[i] < 'a' || str[i] > 'z') {
        if (str[i] < 'A' || str[i] > 'Z') {
          if (str[i] != '@' && str[i] != '.') {
            throw(SevenStream::exception("InvalidMail"));
          }
        }
      }
    }
  }
  return;
}
void CheckPrivilege(const char *str) {
  int len = strlen(str);
  int res = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidPrivilege"));
    }
    res *= 10;
    res += (str[i] - '0');
  };
  if (res > 10) {
    throw(SevenStream::exception("InvalidPrivilege"));
  }
  return;
};
void CheckTrainID(const char *str) {
  int len = strlen(str);
  if (len > 20 || (!len)) {
    throw(SevenStream::exception("InvalidTrainID"));
  }
  if ((str[0] < 'a') || str[0] > 'z') {
    if ((str[0] < 'A') || str[0] > 'Z') {
      throw(SevenStream::exception("InvalidTrainID"));
    }
  }
  for (int i = 0; i < len; ++i) {
    if (str[i] < '0' || str[i] > '9') {
      if (str[i] < 'a' || str[i] > 'z') {
        if (str[i] < 'A' || str[i] > 'Z') {
          if (str[i] != '_') {
            throw(SevenStream::exception("InvalidTrainID"));
          }
        }
      }
    }
  }
  return;
}
void CheckStatNum(const char *str) {
  int len = strlen(str);
  int res = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidStaNum"));
    }
    res *= 10;
    res += (str[i] - '0');
  };
  if (res < 2 || res > 100) {
    throw(SevenStream::exception("InvalidStaNum"));
  }
  return;
}
void CheckStation(const char *str) {
  int len = strlen(str);
  int total = 0;
  for (int i = 0; i < len; i++, total++) {
    if (str[i] > 0) {
      long long byte1 = static_cast<unsigned char>(str[i]);
      long long trans = byte1;
      int how_many = 0;
      for (int i = 8; i; i--) {
        if (byte1 & (1 << (i - 1))) {
          how_many++;
        } else {
          break;
        }
      }
      if (how_many == 0) {
        how_many = 1;
        trans = byte1;
      }
      if (how_many == 2) {
        if (i + 1 >= len) {
          throw(SevenStream::exception("InvalidStationName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 192;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
      }
      if (how_many == 3) {
        if (i + 2 >= len) {
          throw(SevenStream::exception("InvalidStationName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 224;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
        trans <<= 6;
        long long byte3 = static_cast<unsigned char>(str[i + 2]);
        trans -= 128;
        trans += byte3;
      }
      if (how_many == 4) {
        if (i + 3 >= len) {
          throw(SevenStream::exception("InvalidStationName"));
        }
        long long byte2 = static_cast<unsigned char>(str[i + 1]);
        trans -= 240;
        trans <<= 6;
        trans += byte2;
        trans -= 128;
        trans <<= 6;
        long long byte3 = static_cast<unsigned char>(str[i + 2]);
        trans -= 128;
        trans <<= 6;
        long long byte4 = static_cast<unsigned char>(str[i + 3]);
        trans += byte4;
        trans -= 128;
      }
      if (trans == 0x0087 || trans == 0x2014 || trans == 0xFF08 ||
          trans == 0xFF09 || trans == 0xFF1A) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x4E00 && trans <= 0x9FFF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x3400 && trans <= 0x4DBF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x20000 && trans <= 0x2A6DF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2A700 && trans <= 0x2B73F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2B740 && trans <= 0x2B81F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2B820 && trans <= 0x2CEAF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2CEB0 && trans <= 0x2EBEF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x30000 && trans <= 0x3134F) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x31350 && trans <= 0x323AF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0xF900 && trans <= 0xFAFF) {
        i += (how_many - 1);
        continue;
      }
      if (trans >= 0x2F800 && trans <= 0x2FA1F) {
        i += (how_many - 1);
        continue;
      }
      throw(SevenStream::exception("InvalidStationName"));
    } else {
      throw(SevenStream::exception("InvalidStationName"));
    }
  }
  if ((total > 10) || (!total)) {
    throw(SevenStream::exception("InvalidStationName"));
  }
  return;
}
void CheckPrice(const char *str) {
  int len = strlen(str);
  int res = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidPrice"));
    }
    res *= 10;
    res += (str[i] - '0');
  };
  if ((!res) || (res > 100000)) {
    throw(SevenStream::exception("InvalidPrice"));
  }
  return;
}
void CheckStartTime(const char *str) {
  int len = strlen(str);
  if (len != 5) {
    throw(SevenStream::exception("InvalidTime"));
  }
  if (str[2] != ':') {
    throw(SevenStream::exception("InvalidTime"));
  }
  int hour = 0;
  int min = 0;
  for (int i = 0; i < 2; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidTime"));
    }
    hour *= 10;
    hour += (str[i] - '0');
  }
  if (hour > 23) {
    throw(SevenStream::exception("InvalidTime"));
  }
  for (int i = 3; i < 5; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidTime"));
    }
    min *= 10;
    min += (str[i] - '0');
  }
  if (min > 59) {
    throw(SevenStream::exception("InvalidTime"));
  }
  return;
}
void CheckInterTime(const char *str) {
  int res = 0;
  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidTime"));
    }
    res += str[i];
  }
  if (res > 10000) {
    throw(SevenStream::exception("InvalidTime"));
  }
  return;
}
void CheckDate(const char *str) {
  int len = strlen(str);
  if (len != 5) {
    throw(SevenStream::exception("InvalidDate"));
  }
  if (str[2] != '-') {
    throw(SevenStream::exception("InvalidDate"));
  }
  int month = 0;
  int day = 0;
  for (int i = 0; i < 2; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidDate"));
    }
    month *= 10;
    month += (str[i] - '0');
  }
  if ((month < 6) || (month > 8)) {
    throw(SevenStream::exception("InvalidDate"));
  }
  for (int i = 3; i < 5; i++) {
    if (str[i] < '0' || str[i] > '9') {
      throw(SevenStream::exception("InvalidDate"));
    }
    day *= 10;
    day += (str[i] - '0');
  }
  if (day > 31) {
    throw(SevenStream::exception("InvalidDate"));
  }
  if ((day == 31) && (month == 6)) {
    throw(SevenStream::exception("InvalidDate"));
  }
  return;
}
void CheckType(const char *str) {
  int len = strlen(str);
  if (len != 1) {
    throw(SevenStream::exception("InvalidType"));
  }
  if (str[0] < 'A' || str[0] > 'Z') {
    throw(SevenStream::exception("InvalidType"));
  }
  return;
}