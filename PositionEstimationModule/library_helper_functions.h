//
// for this library usage only

#ifndef LIBRARY_HELPER_FUNCTION_H_
#define LIBRARY_HELPER_FUNCTION_H_

namespace position_estimation_module{

void AddAllFilesInDir(std::vector<std::string>& filenames, std::string dir_base);
wchar_t *string2wchar_t(const std::string &str);
std::string WCHAR2string(const std::wstring& s);

}
#endif //LIBRARY_HELPER_FUNCTION_H_