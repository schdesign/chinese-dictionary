// chinesedictionary.h
// Copyright (C) 2023 Alexander Karpeko

#ifndef CHINESEDICTIONARY_H
#define CHINESEDICTIONARY_H

#include "translator.h"
#include "ui_chinesedictionary.h"
#include <QMainWindow>

class ChineseDictionary : public QMainWindow, private Ui::ChineseDictionary
{
    Q_OBJECT

public:
    explicit ChineseDictionary(QWidget *parent = nullptr);

private slots:
    void about();
    void chineseToRussian();
    void monotonePinyin();
    void pinyin();
    void quit();
    void russianToChinese();

private:
    unsigned long monotonePinyinWordIndex;
    unsigned long pinyinWordIndex;
    unsigned long russianWordIndex;
    QString previousMonotonePinyinWord;
    QString previousPinyinWord;
    QString previousRussianWord;
    Translator translator;
    TranslationObject translationObject;
    std::vector<TranslationObject> tmpObjects;
};

#endif  // CHINESEDICTIONARY_H
