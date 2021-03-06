#ifndef VSX_ARGVECTOR_H
#define VSX_ARGVECTOR_H

#include <vsx_avector.h>


class vsx_argvector
{
  vsx_avector<vsx_string> data;
public:
  vsx_string& operator[](size_t index)
  {
    return data[index];
  }

  size_t size()
  {
    return data.size();
  }

  size_t push_back(const vsx_string& val)
  {
    data.push_back(val);
  }

  bool has_param(const vsx_string& param)
  {
    int data_num_elements = size();
    if (data_num_elements == 0) return false;
    for (size_t i = 0; i < data_num_elements; i++)
    {
      if (data[i] == "-"+param)
      {
        return true;
      }
    }
    return false;
  }

  bool has_param_with_value(const vsx_string& param)
  {
    int data_num_elements = size();
    if (data_num_elements == 0) return false;
    for (size_t i = 0; i < data_num_elements; i++)
    {
      if (data[i] == "-"+param)
      {
        if (i+1 < data_num_elements)
        {
          if (data[i+1][0] != '-')
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  vsx_string get_param_value(const vsx_string& param)
  {
    int data_num_elements = size();
    if (data_num_elements == 0) return "";
    for (size_t i = 0; i < data_num_elements; i++)
    {
      if (data[i] == "-"+param)
      {
        if (i+1 < data_num_elements)
        {
          if (data[i+1][0] != '-')
          {
            return data[i+1];
          }
        }
      }
    }
    return "";
  }

  vsx_argvector() {}
};

#endif // VSX_ARGVECTOR_H
