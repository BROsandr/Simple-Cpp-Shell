#include <iostream>

#include <fplus/fplus.hpp>

void print_prompt() { std::cout << "CSCI3120>"; }

std::string get_input() {
  std::string input;
  std::getline(std::cin, input);
  return input;
}

namespace shell {
void exit() { std::exit(EXIT_SUCCESS); }
}  // namespace shell

int main() {
  std::vector<std::string> entered_input;
  std::vector<std::string> command_history;

  while (true) {
    print_prompt();

    auto input_split{
        fplus::fwd::apply(get_input(), fplus::fwd::split_words(false))};

    fplus::fwd::apply(input_split, fplus::fwd::take(1),
                      fplus::fwd::keep_if(fplus::fwd::is_equal(std::string{"exit"})),
                      [](const auto &arg) {
                        if (fplus::is_not_empty(arg)) shell::exit();
                      });

    std::cout << fplus::fwd::apply(
        input_split, fplus::fwd::keep_if(fplus::fwd::is_not_equal(std::string{"history"})),
        fplus::fwd::drop_if(fplus::fwd::is_prefix_of(std::string{"!"})), fplus::fwd::show());
  }
}
