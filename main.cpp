/*
MIT License

Copyright (c) 2018 Klaralvdalens Datakonsult AB (KDAB)
Author: Sergio Martins <sergio.martins@kdab.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileInfo>
#include <QtGui/private/qcssparser_p.h>
#include <QWidget>


static QtMessageHandler s_originalMessageHandler;
static bool s_warningsDetected = false;

void noWarningsMessageHandler(QtMsgType t, const QMessageLogContext &context, const QString &msg)
{
    // This is used in silent mode.
    if (t == QtWarningMsg)
        s_warningsDetected = true;

    s_originalMessageHandler(t, context, msg);
}

static bool lintStyleSheet(const QString &css, bool isFile, bool syntaxOnly)
{
    QCss::Parser parser(css, isFile);
    QCss::StyleSheet ss;

    s_originalMessageHandler = qInstallMessageHandler(noWarningsMessageHandler);
    s_warningsDetected = false;
    const bool success = parser.parse(&ss) && (!s_warningsDetected || syntaxOnly);
    qInstallMessageHandler(s_originalMessageHandler);

    if (success) {
        return true;
    } else {
        if (isFile)
            qWarning() << QStringLiteral("Invalid stylesheet for %1").arg(css);
        else
            qWarning() << "Invalid stylesheet";
       return false;
    }
}

int main(int argv, char** argc)
{
    QApplication app(argv, argc);
    QCoreApplication::setApplicationName("qsslint");
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(QLatin1String("Qt stylesheet syntax verifier"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption syntaxOnlyOption(QStringList() << "s" << "syntax-only", QLatin1String("Only validate syntax, no semantics"));
    parser.addOption(syntaxOnlyOption);

    QCommandLineOption pedanticOption(QStringList() << "p" << "pedantic", QLatin1String("Don't allow css outside of the recommended QSS subset"));
    parser.addOption(pedanticOption);

    parser.addPositionalArgument(QLatin1String("files"), QLatin1String("list of qss files to verify"));

    parser.process(app);

    const auto files = parser.positionalArguments();

    if (parser.isSet(pedanticOption)) {
        qWarning() << "\nThe only recommended qss subset is the empty set.\n"
"Qt StyleSheets are not recommended and fall very short of QStyle.\n"
"They're also being considered for deprecation or removal in Qt 6.\n"
"Please avoid using stylesheets in any new project.\n";
        return 1;
    }

    if (files.isEmpty()) {
        parser.showHelp(-1);
    }

    const bool isSyntaxOnly = parser.isSet(syntaxOnlyOption);
    bool success = true;
    for (const QString &filename : files) {
        success &= lintStyleSheet(filename, /*isFile=*/ true, isSyntaxOnly);
    }

    return success ? 0 : 1;
}
