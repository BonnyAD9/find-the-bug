# C++ Bug 1

You can see full buggy example in the folder [buggy](buggy) and the fixed code
with explanation of the bug in the folder [fixed](fixed).

## The buggy code
```c++
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
```

## Expected behaviour
```shell
$ ./main some args
-i some -i args
```

## Actual behaviour
```shell
$ ./main some args
P3άYY some -i args
```
