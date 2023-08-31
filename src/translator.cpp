// translator.cpp
// Copyright (C) 2023 Alexander Karpeko

#include "translator.h"
#include <QByteArray>
#include <QFile>
#include <QStringList>
#include <QVector>
#include <set>

bool Translator::chineseWordToObject(const QString &str, TranslationObject &tObj)
{
    TranslationObject emptyObject;
    emptyObject.hskNumber = 0;
    tObj = emptyObject;

    auto search = chineseWordToIndex.find(str);
    if (search != chineseWordToIndex.end()) {
        tObj = translationObjects[search->second];
        return true;
    }

    return false;
}

void Translator::init(const QString &text)
{
    QStringList stringList = text.split("\n");

    TranslationObject tObj;
    int wordNumber = 1;

    for (auto s : stringList) {
        if (s.trimmed().isEmpty())
            continue;

        int number = stringToObject(s, tObj);
        if (wordNumber != number)
            throw QString("Translator stringToObject() error: %1 %2 %3")
                .arg(wordNumber).arg(number).arg(tObj.chineseWord);
        wordNumber++;

        tObj.hskNumber = 6;
        translationObjects.push_back(tObj);
    }

    for (unsigned long i = 0; i < translationObjects.size(); i++)
        if (!translationObjects[i].chineseWord.isEmpty())
            chineseWordToIndex[translationObjects[i].chineseWord] = i;

    for (unsigned long i = 0; i < translationObjects.size(); i++)
        if (!translationObjects[i].pinyinWord.isEmpty()) {
            QString str = translationObjects[i].pinyinWord.remove(' ');
            str = monotone(str);
            monotonePinyinWordToIndex[str].push_back(i);
        }

    for (unsigned long i = 0; i < translationObjects.size(); i++)
        if (!translationObjects[i].pinyinWord.isEmpty()) {
            QString str = translationObjects[i].pinyinWord.remove(' ');
            pinyinWordToIndex[str].push_back(i);
        }

    for (unsigned long i = 0; i < translationObjects.size(); i++)
        if (!translationObjects[i].russianWords.isEmpty()) {
            QString str = translationObjects[i].russianWords;
            str.replace(QString(","), QString(";"));
            QStringList stringList = str.split(";", Qt::SkipEmptyParts);
            for (auto s : stringList) {
                if (s.trimmed().isEmpty())
                    continue;
                russianWordToIndex[s.trimmed()].push_back(i);
            }
        }
}

bool Translator::isCyrillic(const QString &str)
{
   if ((str >= QString("А") && str <= QString("Я")) ||
       (str >= QString("а") && str <= QString("я")) ||
       (str == QString("Ё") && str == QString("ё")))
       return true;

    return false;
}

bool Translator::isLeftBracket(const QString &str)
{
   if (str == QString("(") || str == QString("["))
       return true;

    return false;
}

bool Translator::isPinyin(const QString &str)
{
    std::set<QString> additionalSet = {
        "ā", "á", "ǎ", "à", "ē", "é", "ě", "è",
        "ī", "í", "ǐ", "ì", "ō", "ó", "ǒ", "ò",
        "ū", "ú", "ǔ", "ù", "ǖ", "ǘ", "ǚ", "ǜ",
        "ü"
    };

    if ((str >= QString("a") && str <= QString("z")) ||
        (additionalSet.find(str) != additionalSet.end()))
        return true;

    return false;
}

bool Translator::isUppercaseLatin(const QString &str)
{
    if (str >= QString("A") && str <= QString("Z"))
        return true;

    return false;
}

QString Translator::monotone(const QString &pinyin)
{
    typedef std::pair<QString, QString> p;

    std::map<QString, QString> tonemap = {
        p("ā", "a"), p("á", "a"), p("ǎ", "a"), p("à", "a"),
        p("ē", "e"), p("é", "e"), p("ě", "e"), p("è", "e"),
        p("ī", "i"), p("í", "i"), p("ǐ", "i"), p("ì", "i"),
        p("ō", "o"), p("ó", "o"), p("ǒ", "o"), p("ò", "o"),
        p("ū", "u"), p("ú", "u"), p("ǔ", "u"), p("ù", "u"),
        p("ǖ", "ü"), p("ǘ", "ü"), p("ǚ", "ü"), p("ǜ", "ü")
    };

    QString monotonePinyin;

    for (auto m : pinyin) {
        QString str(m);
        if (tonemap.find(str) != tonemap.end())
            monotonePinyin += tonemap[str];
        else
            monotonePinyin += str;
    }

    return monotonePinyin;
}

bool Translator::monotonePinyinWordToObjects(const QString &str, TranslationObjects &tObjects)
{
    tObjects.clear();

    QString str2 = str;
    str2 = str2.remove(' ');
    str2 = monotone(str2);

    auto search = monotonePinyinWordToIndex.find(str2);
    if (search != monotonePinyinWordToIndex.end()) {
        for (auto index : search->second)
            tObjects.push_back(translationObjects[index]);
        return true;
    }

    return false;
}

bool Translator::pinyinWordToObjects(const QString &str, TranslationObjects &tObjects)
{
    tObjects.clear();

    QString str2 = str;
    str2 = str2.remove(' ');

    auto search = pinyinWordToIndex.find(str2);
    if (search != pinyinWordToIndex.end()) {
        for (auto index : search->second)
            tObjects.push_back(translationObjects[index]);
        return true;
    }

    return false;
}

void Translator::readAdditionalFile(const char *filename,  unsigned long hskNumber)
{
    QString text;
    readFile(filename, text);
    updateHskNumbers(text, hskNumber);
}

void Translator::readFile(const char *filename, QString &text)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        throw QString("Translator readFile() error");
    QByteArray array = file.readAll();
    file.close();

    text.clear();
    text.append(array);
}

void Translator::readMainFile(const char *filename)
{
    QString text;
    readFile(filename, text);
    init(text);
}

bool Translator::russianWordToObjects(const QString &str, TranslationObjects &tObjects)
{
    tObjects.clear();

    auto search = russianWordToIndex.find(str);
    if (search != russianWordToIndex.end()) {
        for (auto index : search->second)
            tObjects.push_back(translationObjects[index]);
        return true;
    }

    return false;
}

int Translator::stringToObject(const QString &str, TranslationObject &tObj)
{
    QString tmpString = str;
    tmpString.replace(QString("\t"), QString(" "));
    QStringList stringList = tmpString.split(" ", Qt::SkipEmptyParts);

    if (stringList.size() < 4)
        return 0;

    for (auto &s : stringList)
        s = s.trimmed();

    int stringNumber = stringList[0].toInt();

    tObj.hskNumber = 0;

    int type = 1;

    for (int i = 1; i < stringList.size(); i++) {
        if (stringList[i].isEmpty())
            continue;
        if (type == 1) {
            tObj.chineseWord = stringList[i];
            type = 2;
            continue;
        }
        if (type == 2) {
            if (isPinyin(QString(stringList[i][0]))) {
                tObj.pinyinWord = stringList[i] + " ";
                type = 3;
                continue;
            }
        }
        if (type == 3) {
            if (isPinyin(QString(stringList[i][0]))) {
                tObj.pinyinWord.append(stringList[i] + " ");
                continue;
            }
            else
                if (isCyrillic(QString(stringList[i][0])) ||
                    isUppercaseLatin(QString(stringList[i][0])) ||
                    isLeftBracket(QString(stringList[i][0]))) {
                    tObj.russianWords = stringList[i] + " ";
                    type = 4;
                    continue;
                }
        }
        if (type == 4) {
            if (isCyrillic(QString(stringList[i][0])) ||
                isUppercaseLatin(QString(stringList[i][0])) ||
                isLeftBracket(QString(stringList[i][0]))) {
                tObj.russianWords.append(stringList[i] + " ");
                continue;
            }
            else {
                tObj.examples = stringList[i] + " ";
                type = 5;
                continue;
            }
        }
        if (type == 5)
                tObj.examples.append(stringList[i] + " ");
    }

    if (type < 4)
        return -type;

    tObj.chineseWord = tObj.chineseWord.trimmed();
    tObj.pinyinWord = tObj.pinyinWord.trimmed();
    tObj.russianWords = tObj.russianWords.trimmed();
    tObj.examples = tObj.examples.trimmed();

    return stringNumber;
}

void Translator::updateHskNumbers(const QString &text, unsigned long hskNumber)
{
    QStringList stringList = text.split("\n");

    TranslationObject tObj;
    int wordNumber = 1;

    for (auto s : stringList) {
        if (s.trimmed().isEmpty())
            continue;

        int number = stringToObject(s, tObj);
        if (wordNumber != number)
            throw QString("Translator stringToObject() error: %1 %2 %3")
                .arg(wordNumber).arg(number).arg(tObj.chineseWord);
        wordNumber++;

        auto search = chineseWordToIndex.find(tObj.chineseWord);
        if (search != chineseWordToIndex.end()) {
            if (hskNumber < translationObjects[search->second].hskNumber)
                translationObjects[search->second].hskNumber = hskNumber;
        }
        else
            throw QString("Chinese word is not found in the dictionary: %1 %2 %3")
                .arg(number).arg(tObj.chineseWord).arg(hskNumber);
    }
}
