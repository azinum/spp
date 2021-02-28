# spp - simple preprocessor

## Usage

The main feature that spp has for now is lambda expressions. The lambda expressions are essentially generated into locally defined functions in the file in which they are used. To show how it works, here is a trivial usage example of lambda expressions:
```c

int main(void) {
  int result = $(int a, int b) -> int {
    return a + b;
  }(5, 6);

  return 0;
}
```

To parse and generate files simply run:
```
spp <source file> <output directory>
```

The source file is read, parsed, processed and then finally generated, with the same name, into the output directory. Upon spp detecting an error, the program will produce an error message and terminate with an exit code that is non-zero. This is for external programs which uses spp, to terminate or do as it wish when something goes wrong.

## Installation

```bash
git clone https://github.com/azinum/spp
cd spp
sudo make install
```
