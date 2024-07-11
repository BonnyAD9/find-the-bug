#include <vector>
#include <iostream>
#include <span>

class ArgEdit {
public:
    /// Preprocesses the arguments.
    ArgEdit(std::span<char*> args) {
        // Add argument `-i` before each argument.
        for (auto arg : args.subspan(1)) {
            add_gen_arg("-i");
            app_args.push_back(arg);
        }
    }

    /// Retrieves the preprocessed arguments.
    std::span<char*> get_args() {
        return app_args;
    }

private:
    /// Create generated argument. Ensure that it is freed at the right time by
    /// saving it into the vector `generated`.
    void add_gen_arg(std::string arg) {
        // Ensure that the string is heap allocated
        arg.reserve(sizeof(std::string));
        generated.push_back(std::move(arg));
        app_args.push_back(generated.rbegin()->data());
    }

    std::vector<char *> app_args;
    std::vector<std::string> generated;
};

int main(int argc, char **argv) {
    // Preprocess the arguments
    ArgEdit args(std::span(argv, argc));

    // Print the new arguments
    for (auto arg : args.get_args()) {
        std::cout << arg << ' ';
    }
    std::cout << std::endl;
}
