
struct CountryInfo {
    string country_name;
    string country_iso_code;
    string country_phone_code;
    string country_time_zone;
    vector<Language> languages;
    
    CountryInfo(const string& name, const string& iso_code, const string& phone_code, const string& time_zone,
                const vector<Language>& langs)
        : country_name(name),
        country_iso_code(iso_code),
        country_phone_code(phone_code),
        country_time_zone(time_zone),
        languages(langs) {
    }
};

class CountryInfoBuilder {
private:
    string country_name;
    string country_iso_code;
    string country_phone_code;
    string country_time_zone;
    vector<Language> languages;
    
public:
    CountryInfoBuilder& setName(const string& name) {
        country_name = name;
        return *this;
    }
    
    CountryInfoBuilder& setIsoCode(const string& iso_code) {
        country_iso_code = iso_code;
        return *this;
    }
    
    CountryInfoBuilder& setPhoneCode(const string& phone_code) {
        country_phone_code = phone_code;
        return *this;
    }
    
    CountryInfoBuilder& setTimeZone(const string& time_zone) {
        country_time_zone = time_zone;
        return *this;
    }
    
    CountryInfoBuilder& setLanguages(const vector<Language>& langs) {
        languages = langs;
        return *this;
    }
    
    CountryInfo build() const {
        return CountryInfo(country_name, country_iso_code, country_phone_code, country_time_zone, languages);
    }
};

void ParseCitySubjson(vector<City>& cities, const Json& json, const CountryInfo& country_info) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({
            city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            country_info.country_phone_code + city_obj["phone_code"s].AsString(),
            country_info.country_name,
            country_info.country_iso_code,
            country_info.country_time_zone,
            country_info.languages
        });
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            {}
        });
        
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        
        CountryInfoBuilder info_builder;
        CountryInfo country_info = info_builder
                                       .setName(country.name)
                                       .setIsoCode(country.iso_code)
                                       .setPhoneCode(country.phone_code)
                                       .setTimeZone(country.time_zone)
                                       .setLanguages(country.languages)
                                       .build();
        
        ParseCitySubjson(cities, country_obj["cities"s], country_info);
    }
}

