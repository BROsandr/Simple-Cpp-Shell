#include <iostream>

#include <fplus/fplus.hpp>

void print_prompt() { std::cout << "CSCI3120>"; }

std::string get_input() {
  std::string input;
  std::getline(std::cin, input);
  return input;
}

namespace shell {
void exit() {
  std::cout << "Exiting";
  std::exit(EXIT_SUCCESS);
}
} // namespace shell

int main() {
  std::vector<std::string> command_history;

  std::function<void(void)> check_command_history_emptiness{
      [&command_history = std::as_const(command_history)]() {
        fplus::fwd::apply(
            command_history,
            fplus::fwd::is_empty(),
            [](const bool &arg) {
              if (arg)
                std::cout << "No commands in history.." << std::endl;
            });
      }};

  while (true) {
    print_prompt();

    std::vector<std::string> input_split{fplus::fwd::apply(
        get_input(),
        fplus::fwd::split_one_of(std::string{" "}, false))};

    if (!fplus::size_of_cont(input_split)) {
      continue;
    }

    fplus::fwd::apply(input_split,
                      fplus::fwd::head(),
                      fplus::fwd::is_equal(std::string{"exit"}),
                      [](const bool &arg) {
                        if (arg)
                          shell::exit();
                      });

    std::string command_to_execute;
    if (fplus::fwd::apply(input_split,
                          fplus::fwd::head(),
                          fplus::fwd::is_equal(std::string{"history"}))) {
      check_command_history_emptiness();

      std::cout << fplus::fwd::apply(
          fplus::zip(
              fplus::fwd::apply(command_history,
                                fplus::fwd::size_of_cont(),
                                fplus::add_to<long long unsigned>(1),
                                fplus::fwd::numbers<long long unsigned>(1)),
              fplus::fwd::apply(
                  command_history,
                  fplus::fwd::transform(fplus::fwd::prepend_elem(':')))),

          fplus::fwd::take_last(10),
          fplus::fwd::reverse(),
          fplus::fwd::transform([](const auto &arg) {
            return fplus::show(fplus::fst(arg)) + fplus::snd(arg);
          }),
          fplus::fwd::show_cont_with_frame("\n", "", ""));
      if (fplus::is_not_empty(command_history))
        std::cout << std::endl;
    } else {
      if (fplus::fwd::apply(input_split,
                                 fplus::fwd::head(),
                                 fplus::fwd::get_segment(0, 2),
                                 fplus::fwd::is_equal(std::string{"!!"}))) {
        check_command_history_emptiness();

        command_to_execute = (fplus::is_empty(command_history))
                             ? ""
                             : fplus::last(command_history);
      } else if (fplus::fwd::apply(input_split,
                                   fplus::fwd::head(),
                                   fplus::fwd::get_segment(0, 1),
                                   fplus::fwd::is_equal(std::string{"!"}))) {
        if (fplus::fwd::apply(
            input_split,
            fplus::fwd::head(),
            fplus::fwd::get_segment(
                1, fplus::size_of_cont(fplus::head(input_split))),
            fplus::fwd::read_value_with_default<unsigned long long>(-1),
            fplus::fwd::is_in_closed_interval<unsigned long long>(
                1, fplus::size_of_cont(command_history)))) {
          command_to_execute = fplus::fwd::apply(
              input_split,
              fplus::fwd::head(),
              fplus::fwd::get_segment(
                  1, fplus::size_of_cont(fplus::head(input_split))),
              fplus::fwd::read_value_with_default<unsigned long long>(-1),
              fplus::subtract(1),
              fplus::fwd::flip::elem_at_idx(command_history));
        } else {
          std::cout << "No such command in command history" << std::endl;
          continue;
        }
      } else {
        command_to_execute = fplus::join(std::string{' '}, input_split);
      }
      std::cout << fplus::append(command_to_execute, std::string{"\n"});
      command_history = fplus::append(command_history, std::vector<std::string>{command_to_execute});
    }
  }
}
