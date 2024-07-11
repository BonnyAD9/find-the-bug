# C++ Bug 1 solution

## Explanation
The bug is in the method `ArgEdit::add_gen_arg`:
```c++
    /// Create generated argument. Ensure that it is freed at the right time by
    /// saving it into the vector `generated`.
    void add_gen_arg(std::string arg) {
        generated.push_back(std::move(arg));
        app_args.push_back(generated.rbegin()->data());
    }
```

The core of the problem is that C++ string may not use any heap memory to store
its contents if the content size is small enough. Instead it will use array
that is stored flat directly in the string.

Example string implementation using this trick:
```c++
class string {
    // this is basically declaring a constant
    enum { _LOCAL_DATA_SIZE = 15 };

    // Pointer to the string contents. It can be either allocated on heap, or
    // it can point to `_local_data`.
    char *_data;
    std::size_t _length;
    union {
        char _local_data[_LOCAL_DATA_SIZE + 1];
        std::size_t _capacity;
    };

    // private methods ...

    bool _is_local() const {
        return _data == _local_data;
    }

    // other private methods ...
public:
    // constructors and other methods ...

    std::size_t capacity() const {
        return _is_local() ? _LOCAL_DATA_SIZE : _capacity;
    }

    // other methods ...
};
```

This means that for small strings, the pointer obtained with
`std::string::data` will be invalid as soon as the memory with the original
string will be invalid.

This will happen when the vector reallocates, and in this case it will.

## Fix
To fix this, we can just make sure that the string will allocate the data on
the heap. This can be achieved by reserving enough space for the string data:
```c++
    /// Create generated argument. Ensure that it is freed at the right time by
    /// saving it into the vector `generated`.
    void add_gen_arg(std::string arg) {
        // Ensure that the string is heap allocated
        arg.reserve(sizeof(std::string));
        generated.push_back(std::move(arg));
        app_args.push_back(generated.rbegin()->data());
    }
```
