// A number of convenient string operations inspired by string methods in C#.
//
// Code written by Jeppe Revall Frisvad, 2008
// Copyright (c) DTU Informatics 2008

#include <string>
#include <list>
#include "string_utils.h"

using namespace std;

string trim(const string& s, const string& wspaces)
{
  int front = 0;
  int back = static_cast<int>(s.size() - 1);
  if(back < front)
    return string("");

  while(wspaces.find(s[front]) != wspaces.npos) ++front;
  while(wspaces.find(s[back]) != wspaces.npos) --back;
  int n = back - front + 1;
  if(n < 1)
    return string("");
  else
    return s.substr(front, n);    
}

string trim(const string& s)
{
  return trim(s, " \t\n\r");
}

void split(const string& s, list<string>& result, const string& delim)
{
  size_t begin = 0;
  size_t end = s.find(delim);
  while(end != s.npos)
  {
    result.push_back(s.substr(begin, end - begin));
    begin = end + delim.size();
    end = s.find(delim, begin);
  }
  if(s.size() > begin)
    result.push_back(s.substr(begin, s.size() - begin));    
}

void split(const string& s, list<string>& result)
{
  return split(s, result, " ");
}

void trim_split(const string& s, list<string>& result, const string& delim)
{
  size_t begin = 0;
  size_t end = s.find(delim);
  while(end != s.npos)
  {
    result.push_back(trim(s.substr(begin, end - begin)));
    begin = end + delim.size();
    end = s.find(delim, begin);
  }
  if(s.size() > begin)
    result.push_back(trim(s.substr(begin, s.size() - begin)));
}

void trim_split(const string& s, list<string>& result)
{
  trim_split(s, result, " ");
}

void get_first(string& s, string& first)
{
  size_t pos = s.find(" ");
  first = s.substr(0, pos);
  if(pos == s.npos)
    s = "";
  else
    s.erase(0, pos + 1);
}

void get_last(string& s, string& last)
{
  size_t pos = s.rfind(" ");
  last = s.substr(pos + 1);
  if(pos == s.npos)
    s = "";
  else
    s.erase(pos + 1);
}
