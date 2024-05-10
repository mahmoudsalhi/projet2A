// ClipboardHelper.h

#ifndef CLIPBOARDHELPER_H
#define CLIPBOARDHELPER_H

#include <QObject>
#include <QClipboard>

class ClipboardHelper : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardHelper(QObject *parent = nullptr);
    Q_INVOKABLE void setText(const QString &text);
};

#endif // CLIPBOARDHELPER_H
