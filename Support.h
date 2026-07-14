#ifndef SUPPORT_H
#define SUPPORT_H

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

class Exception
{
private:
  std::string name;
  std::string err_name;
  int err_code = 0;

public:
  Exception() = default;
  Exception(std::string &name, std::string &err_name, int err_code)
  {
    this->name = name;
    this->err_name = err_name;
    this->err_code = err_code;
  }
  std::string get_name() { return this->name; }
  std::string get_err_name() { return this->err_name; }
  int get_err_code() { return this->err_code; }
  ~Exception() = default;
};

class check
{
public:
  check() = default;
  ~check() = default;

  template <typename T>
  static int ck(const char *name, T ret, T f_value)
  {
    if (ret == f_value)
    {
      std::string s_name = name;
      std::string err_name = std::strerror(errno);
      Exception err(s_name, err_name, errno);
      throw err;
    }
    std::printf("success %s\n", name);
    return 0;
  }

  template <typename T>
  static int ck_ENOENT(const char *name, T ret, T f_value)
  {
    if (ret == f_value && errno != ENOENT)
    {
      std::string s_name = name;
      std::string err_name = std::strerror(errno);
      Exception err(s_name, err_name, errno);
      throw err;
    }
    std::printf("success %s\n", name);
    return 0;
  }

  template <typename T>
  static int ck(const std::string name, T ret, T f_value)
  {
    if (ret == f_value)
    {
      std::string s_name = name;
      std::string err_name = std::strerror(errno);
      Exception err(s_name, err_name, errno);
      throw err;
    }
    std::printf("success %s\n", name.c_str());
    return 0;
  }

  template <typename T>
  static int ck_r(const std::string name, T ret, T f_value)
  {
    if (ret != f_value)
    {
      std::string s_name = name;
      std::string err_name = std::strerror(errno);
      Exception err(s_name, err_name, errno);
      throw err;
    }
    std::printf("success %s\n", name.c_str());
    return 0;
  }
};

#endif // SUPPORT_H
