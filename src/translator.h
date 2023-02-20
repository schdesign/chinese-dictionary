// translator.h
// Copyright (C) 2023 Alexander Karpeko

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <map>
#include <QString>
#include <vector>

struct TranslationObject
{
    unsigned long hskNumber;
    QString chineseWord;
    QString pinyinWord;
    QString russianWords;
    QString examples;
};

class Translator
{
typedef std::vector<TranslationObject> TranslationObjects;

public:
    bool chineseWordToObject(const QString &str, TranslationObject &tObj);
    bool monotonePinyinWordToObjects(const QString &str, TranslationObjects &tObjects);
    bool pinyinWordToObjects(const QString &str, TranslationObjects &tObjects);
    void readAdditionalFile(const char *filename, unsigned long hskNumber);
    void readMainFile(const char *filename);
    bool russianWordToObjects(const QString &str, TranslationObjects &tObjects);

private:
    void init(const QString &text);
    bool isCyrillic(const QString &str);
    bool isLeftBracket(const QString &str);
    bool isPinyin(const QString &str);
    bool isUppercaseLatin(const QString &str);
    QString monotone(const QString &pinyin);
    void readFile(const char *filename, QString &text);
    int stringToObject(const QString &str, TranslationObject &tObj);
    void updateHskNumbers(const QString &text, unsigned long hskNumber);

private:
    std::map<QString, unsigned long> chineseWordToIndex;
    std::map<QString, std::vector<unsigned long>> monotonePinyinWordToIndex;
    std::map<QString, std::vector<unsigned long>> pinyinWordToIndex;
    std::map<QString, std::vector<unsigned long>> russianWordToIndex;
    std::vector<TranslationObject> translationObjects;
};

#endif  // TRANSLATOR_H
