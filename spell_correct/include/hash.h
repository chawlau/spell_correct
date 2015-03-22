#ifndef __HASH_H
#define __HASH_H
#include<iostream>
#include<hash_map>
#include<set>
struct CMyhash
{
    int operator()(const std::string& obj)const
    {
        __gnu_cxx::hash<const char*> chash;
        return static_cast<int>(chash(obj.c_str()));
    }
};
typedef __gnu_cxx::hash_map<std::string,std::set< std::pair<std::string,int> >,CMyhash> strhash;
typedef __gnu_cxx::hash_map<std::string,std::set< std::pair<std::string,int> >,CMyhash>::iterator striter;
typedef __gnu_cxx::hash_map<std::string,std::string,CMyhash> hash_record;
typedef __gnu_cxx::hash_map<std::string,std::string,CMyhash>::iterator piter;
#endif
