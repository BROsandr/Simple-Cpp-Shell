#include "fplus/fplus.hpp"

void print_prompt() {
    std::cout << "CSCI3120>";
}

std::string get_input() {
    std::string input;
    std::cin >> input;
    return input;
}

namespace shell {
    void exit() { std::exit(EXIT_SUCCESS); }
}

void run() {
    std::vector<std::string> entered_input;
    std::vector<std::string> command_history;

    while (true) {
        print_prompt();

        auto input_split{fplus::fwd::apply(
                get_input(),
                fplus::fwd::split_words(false))};

        fplus::fwd::apply(
                input_split,
                fplus::fwd::take(1),
                fplus::fwd::keep_if(fplus::fwd::is_equal("exit")),
                [](const auto &arg) {
                  if(fplus::is_empty(arg))
                      shell::exit();
                }
        );

        fplus::fwd::apply(
                input_split,
                fplus::fwd::keep_if(fplus::fwd::is_not_equal("history")),
                fplus::fwd::keep_if(fplus::fwd::is_prefix_of("!")),
                [&entered_input, &command_history](const auto &arg) {
                  fplus::append(entered_input, arg);
                  fplus::append(command_history, arg);
                }
        );
    }
}

int main() {
    run();
}
