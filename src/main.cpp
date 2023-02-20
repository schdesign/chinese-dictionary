// main.cpp
// Copyright (C) 2023 Alexander Karpeko

#include "chinesedictionary.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChineseDictionary w;
    w.show();
    return a.exec();
}
