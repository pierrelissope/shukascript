# ShukaScript

ShukaScript is a programming language designed to make learning programming both accessible and enjoyable. This project is developed in C++ by Pierre LISSOPE (alias shukabiat).

## Features

- **Intuitive Syntax**: Inspired by popular languages for quick adoption.
- **Interactive Interpreter**: Execute your code in real-time.
- **Basic Data Types**: Support for integers, floats, strings, and booleans.
- **Control Structures**: Support for conditional statements (if) and loops (while).
- **Custom Functions**: Define and call your own functions.
- **Bulletins Functions**: Already integrated functions that you can use (random, return, print).
- **Easy Types**: automatically managed types.. however for the functions return values you have choice between (int, string, float and variant(the best))

## Compilation

### Requirements

- Compile using a modern C++ compiler that supports C++11 standard or higher.
- Use CMake for managing the project build.

### Building the Project

To build the ShukaScript project, follow these steps:

1. Clone the repository:
```bash
git clone https://github.com/shukabiat/shukascript.git
cd shukascript
```
2. Build the project:
```bash
make
```

### How to use it ?

```bash
./shukascript file.shk
```

### Examples

```bash

global_var = 10;

function variant fonctionincroyable()
{
    print("i have no real utility");
}

function int addition(a, b)
{
    return(a + b);
}

function variant main(choice)
{
    if ((choice == 2) || (choice == 4)) {
        print("win");
    }
    if ((choice == 1) || (choice == 3)) {
        print("loose");
    }
    print(addition(5, 10));

    random_number = random(5, 10);
    print("the random number is: ", random_number);

    temp_var = random_number;

    while (temp_var < 20) {
        temp_var = temp_var + 1;
        print("tour de boucle");
    }
    fonctionincroyable();
    print("this is the end", " ", "hope u enjoyed");
    print("MADE BY SHUKABIAT");
}

main(3);
print(global_var + (7 * 9));
```

## License
ShukaScript is licensed under the MIT License. See the LICENSE file for more information.
