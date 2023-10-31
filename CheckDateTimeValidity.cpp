void CheckRange(int value, int min_value, int max_value, const string& error_message) {
    if (value < min_value || value > max_value) {
        throw domain_error(error_message);
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckRange(dt.year, MIN_YEAR, MAX_YEAR, "year is out of range"s);
    CheckRange(dt.month, MIN_MONTH, MAX_MONTH, "month is out of range"s);
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array<int, 12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    CheckRange(dt.day, MIN_DAY, month_lengths[dt.month - 1], "day is out of range"s);
    CheckRange(dt.hour, MIN_HOUR, MAX_HOUR, "hour is out of range"s);
    CheckRange(dt.minute, MIN_MIN, MAX_MIN, "minute is out of range"s);
    CheckRange(dt.second, MIN_SEC, MAX_SEC, "second is out of range"s);
}

const static int MAX_YEAR = 9999;
const static int MIN_YEAR = 1;
const static int MAX_MONTH = 12;
const static int MIN_MONTH = 1;
const static int MIN_DAY = 1;
const static int MAX_HOUR = 23;
const static int MIN_HOUR = 0;
const static int MAX_MIN = 59;
const static int MIN_MIN = 0;
const static int MAX_SEC = 59;
const static int MIN_SEC = 0;
