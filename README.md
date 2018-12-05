# qsslint

Linter for Qt stylesheet files. A tool that simply returns 0 if your stylesheet is valid, *non-zero* otherwise.
Ideal for hooking up in your build system or CI.

Example usage:
```
$ qsslint mystylesheet.qss
QCssParser::parseHexColor: Unknown color name '#00000'
Invalid stylesheet for mystylesheet.qss
```
