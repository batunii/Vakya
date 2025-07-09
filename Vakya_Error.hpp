#include <exception>
#include <string>
class vakya_error : public std::exception {
  size_t error_location = 0;
  std::string message;

public:
  vakya_error(std::string &&message, size_t err)
      : message(message + " at loc : " + std::to_string(err)) {}
  const char *what() const noexcept { return message.c_str(); }
};
