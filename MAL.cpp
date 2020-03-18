
#include<iostream>
#include<cstdint>
#include<string>
#include<vector>
#include<exception>
#include<ctime>
#include<map>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>

//#include<gtkmm-3.0/gtkmm/window.h>
//#include<gtkmm-3.0/gtkmm/button.h>

using namespace std;
namespace pt = boost::property_tree;
namespace dt = boost::gregorian;

struct Anime{
        uint32_t id;
        string name;
        string type;
        uint16_t episodes;
        uint16_t watched;
        time_t start;
        time_t end;
        uint8_t score;
        string status;
        uint8_t rewatchs;
        time_t season;
        Anime();
        Anime(pt::ptree::value_type& v);
    private:
        time_t getTime(string);
        time_t getSeasonTime(string);
};

time_t Anime::getTime(string s){
    uint16_t day, month, year;
    sscanf(s.c_str(),"%d-%d-%d", &year, &month, &day);
    if(year < 1900) year = 2000;
    if(month == 0) month++;
    if(day == 0) day++;
    tm t = dt::to_tm(dt::date(year, month, day));
    return mktime(&t); 
}

time_t Anime::getSeasonTime(string s){
    const static map<string, uint16_t> seasons = {{"Wi", 1}, {"Sp", 4}, {"Su", 7}, {"Fa", 10}};
    uint16_t year, month;
    if(s[0]=='?' || s.length() < 2){
        year = 2100;
        month = 1;
    }else{
        year = stoi(s.substr(2, 2));
        year += year > 50 ? 1900 : 2000;
        month = seasons.at(s.substr(0, 2));
    }
    if(year > 2110 || year < 1970)
        cout<<"but how"<<endl;
    tm t = dt::to_tm(dt::date(year, month, 1));
    return mktime(&t); 
}

Anime::Anime(pt::ptree::value_type& v){
    id = v.second.get<uint32_t>("series_animedb_id");
    name = v.second.get<string>("series_title");
    type = v.second.get<string>("series_type");
    episodes = v.second.get<uint16_t>("series_episodes");
    watched = v.second.get<uint16_t>("my_watched_episodes");
    start = getTime(v.second.get<string>("my_start_date"));
    end = getTime(v.second.get<string>("my_finish_date"));
    score = v.second.get<uint8_t>("my_score");
    status = v.second.get<string>("my_status");
    rewatchs = v.second.get<uint8_t>("my_times_watched");
    season = getSeasonTime(v.second.get<string>("my_tags"));
}

int main(){
    try{
        pt::ptree tree;
        pt::read_xml("animeList.xml", tree);
        vector<Anime> animu;
        for(pt::ptree::value_type& v : tree.get_child("myanimelist"))
            if(v.first == "anime")
                animu.push_back(Anime(v));
        cout<<animu.size()<<endl;
    }
    catch(exception& e){
        cout<<e.what()<<endl;
    }
    system("pause");
    return 0;
}

