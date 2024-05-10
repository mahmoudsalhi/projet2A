// clipboardhelper.cpp

#include "clipboardhelper.h"
#include <QClipboard>
#include <QGuiApplication>



ClipboardHelper::ClipboardHelper(QObject *parent) : QObject(parent) {}

void ClipboardHelper::setText(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}
