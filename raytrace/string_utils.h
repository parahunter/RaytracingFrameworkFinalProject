// A number of convenient string operations inspired by string methods in C#.
//
// Code written by Jeppe Revall Frisvad, 2008
// Copyright (c) DTU Informatics 2008

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

std::string trim(const std::string& s, const std::string& wspaces);
std::string trim(const std::string& s);
void split(const std::string& s, std::list<std::string>& result, const std::string& delim);
void split(const std::string& s, std::list<std::string>& result);
void trim_split(const std::string& s, std::list<std::string>& result, const std::string& delim);
void trim_split(const std::string& s, std::list<std::string>& result);
void get_first(std::string& s, std::string& first);
void get_last(std::string& s, std::string& last);

#endif // STRING_UTILS_H
