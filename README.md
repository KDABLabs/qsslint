# qsslint - a linter for Qt stylesheet files
A tool that simply returns 0 if your stylesheet is valid, *non-zero* otherwise.
Ideal for hooking up in your build system or CI.

Example usage:
```
$ qsslint mystylesheet.qss
QCssParser::parseHexColor: Unknown color name '#00000'
Invalid stylesheet for mystylesheet.qss
```

```
$ qsslint --help
Usage: qsslint [options] files

Qt stylesheet syntax verifier

NOTE: Avoid using Qt style sheets. Qt style sheets are pratically unmaintained and are being considered for deprecation or removal in Qt 6. Often they aren't flexible enough to implement complex styles and you only realize that when it's too late to rewrite with QStyle. Mixing style sheets with proxy styles would solve the above but it's currently not well supported (see https://codereview.qt-project.org/#/c/218791/ for proposed solution).
See also QTBUG-68671 for less drastic Qt 6 plans.

Options:
  -h, --help         Displays this help.
  -v, --version      Displays version information.
  -s, --syntax-only  Only validate syntax, not semantics

Arguments:
  files              list of qss files to verify
```
