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
  command_history = {"dgdg", "hell", "hdfdf"};

  while (true) {
    print_prompt();

    auto input_split{fplus::fwd::apply(
        get_input(), fplus::fwd::split_one_of(std::string{" "}, false))};

    fplus::fwd::apply(input_split, fplus::fwd::head(),
                      fplus::fwd::is_equal(std::string{"exit"}),
                      [](const auto &arg) {
                        if (arg) shell::exit();
                      });

    auto shell_command{fplus::fwd::apply(
        input_split,
        fplus::fwd::keep_if(fplus::fwd::is_not_equal(std::string{"history"})),
        fplus::fwd::drop_if(fplus::fwd::is_prefix_of(std::string{"!"})),
        fplus::fwd::show())};

    if (fplus::fwd::apply(input_split, fplus::fwd::head(),
                          fplus::fwd::is_equal(std::string{"history"}))) {
      fplus::fwd::apply(command_history, fplus::fwd::is_empty(),
                        [](const auto &) {
                          std::cout << "No command in history.." << std::endl;
                        });

      std::cout << fplus::fwd::apply(
          fplus::zip(
              fplus::fwd::apply(command_history, fplus::fwd::size_of_cont(),
                                fplus::add_to<long long unsigned>(1),
                                fplus::fwd::numbers<long long unsigned>(1)),
              fplus::fwd::apply(
                  command_history,
                  fplus::fwd::transform(fplus::fwd::prepend_elem(':')))),

          fplus::fwd::take_last(10), fplus::fwd::reverse(),
          fplus::fwd::transform([](const auto &arg) {
            return fplus::show(fplus::fst(arg)) + fplus::snd(arg);
          }),
          fplus::fwd::show());
    } else if (fplus::fwd::apply(input_split, fplus::fwd::head(),
                                 fplus::fwd::get_segment(0, 2),
                                 fplus::fwd::is_equal(std::string{"!!"}))) {
      std::cout << "here";
    }
  }
}
