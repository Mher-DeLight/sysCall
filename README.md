#  sysCall
sysCall is a language with C-inspired syntax that transpiles to Bash. Its main purpose is to prevent direct exposure to the ugly and inconsistent Bash language by transpiling code to it.

## Language
sysCall syntax is C-inspired but adjusted to fit Bash. The biggest differences are its lack of a `main()` function and its native `string` type. Used commands must be included beforehand via `#include <command>`. Here's an example:
```cpp
#include <read>
#include <echo>

string name = "";
echo("What is your name?");
read(name);
echo("Hello " + name);
```