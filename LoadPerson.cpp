#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <string_view>

struct DBInfo {
    std::string_view db_name;
    int db_connection_timeout = 0;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

struct QueryInfo {
    int min_age;
    int max_age;
    std::string name_filter;
};

optional <DBHandler> ConnectToDatabase(const DBInfo& db_info) {
    DBConnector connector(db_info.db_allow_exceptions, db_info.db_log_level);
     DBHandler db;
    if (db_info.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_info.db_name, db_info.db_connection_timeout);
    } else {
        db = connector.Connect(db_info.db_name, db_info.db_connection_timeout);
    }
    if (!db_info.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }
    return db;
}

std::vector<Person> ExecutePersonQuery(const DBHandler& db, const QueryInfo& query_info) {
    std::ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << query_info.min_age << " and "s << query_info.max_age << " "s
              << "and Name like '%"s << db.Quote(query_info.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<std::string, int>(query)) {
        persons.push_back({std::move(name), age});
    }
    return persons;
}

std::vector<Person> LoadPersons(const DBInfo& db_info, const QueryInfo& query_info) {
    optional <DBHandler> db = ConnectToDatabase(db_info);
    
    if (!db) {
        return {};
    }

    return ExecutePersonQuery(db, query_info);
}

int main() {
    std::string_view db_name = "test.db";
    int db_connection_timeout = 5;
    bool db_allow_exceptions = true;
    DBLogLevel db_log_level = DBLogLevel::Info;
    int min_age = 10;
    int max_age = 80;
    std::string name_filter = "Nik";

    DBInfo db_info{db_name, db_connection_timeout, db_allow_exceptions, db_log_level};
    QueryInfo query_info{min_age, max_age, name_filter};

    std::vector<Person> persons = LoadPersons(db_info, query_info);

    for (const Person& person : persons) {
        std::cout << "Name: " << person.name << ", Age: " << person.age << std::endl;
    }

    return 0;
}
