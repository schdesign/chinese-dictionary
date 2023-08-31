// chinesedictionary.cpp
// Copyright (C) 2023 Alexander Karpeko

#include "chinesedictionary.h"
#include "translator.h"
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

ChineseDictionary::ChineseDictionary(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(chineseToRussianButton, SIGNAL(clicked()), this, SLOT(chineseToRussian()));
    connect(monotonePinyinButton, SIGNAL(clicked()), this, SLOT(monotonePinyin()));
    connect(pinyinButton, SIGNAL(clicked()), this, SLOT(pinyin()));
    connect(russianToChineseButton, SIGNAL(clicked()), this, SLOT(russianToChinese()));

    pinyinWordIndex = 0;
    russianWordIndex = 0;

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    const char *additionalFiles[5] = {
        "hskwords1.txt", "hskwords2.txt", "hskwords3.txt",
        "hskwords4.txt", "hskwords5.txt"
    };

    try {
        translator.readMainFile("hskwords6.txt");

        for (unsigned long i = 0; i < 5; i++)
            translator.readAdditionalFile(additionalFiles[i], i + 1);
    }
    catch (const QString &e) {
        QMessageBox::warning(this, tr("Error"), e);
    }
}

void ChineseDictionary::about()
{
    QMessageBox::information(this, tr("About Chinese Dictionary"),
        tr("Chinese Dictionary\n""Copyright (C) 2023 Alexander Karpeko"));
}

void ChineseDictionary::chineseToRussian()
{
    QString chineseWord = chineseWordLineEdit->text();

    if (translator.chineseWordToObject(chineseWord, translationObject)) {
        pinyinWordLineEdit->setText(translationObject.pinyinWord);
        russianWordLineEdit->setText("");
        hskLineEdit->setText(QString::number(translationObject.hskNumber));
        russianWordsLineEdit->setText(translationObject.russianWords);
        examplesLineEdit->setText(translationObject.examples);
    }
}

void ChineseDictionary::monotonePinyin()
{
    TranslationObject emptyObject;
    emptyObject.hskNumber = 0;
    translationObject = emptyObject;

    QString monotonePinyinWord = monotonePinyinWordLineEdit->text();

    if (monotonePinyinWord == previousMonotonePinyinWord) {
        monotonePinyinWordIndex++;
        if (!tmpObjects.empty()) {
            monotonePinyinWordIndex %= tmpObjects.size();
            translationObject = tmpObjects[monotonePinyinWordIndex];
        }
        else
            monotonePinyinWordIndex = 0;
    }
    else {
        previousMonotonePinyinWord = monotonePinyinWord;
        monotonePinyinWordIndex = 0;
        if (translator.monotonePinyinWordToObjects(monotonePinyinWord, tmpObjects))
            translationObject = tmpObjects[monotonePinyinWordIndex];
    }

    chineseWordLineEdit->setText(translationObject.chineseWord);
    pinyinWordLineEdit->setText(translationObject.pinyinWord);
    russianWordLineEdit->setText("");
    hskLineEdit->setText(QString::number(translationObject.hskNumber));
    russianWordsLineEdit->setText(translationObject.russianWords);
    examplesLineEdit->setText(translationObject.examples);
}

void ChineseDictionary::pinyin()
{
    TranslationObject emptyObject;
    emptyObject.hskNumber = 0;
    translationObject = emptyObject;

    QString pinyinWord = pinyinWordLineEdit->text();

    if (pinyinWord == previousPinyinWord) {
        pinyinWordIndex++;
        if (!tmpObjects.empty()) {
            pinyinWordIndex %= tmpObjects.size();
            translationObject = tmpObjects[pinyinWordIndex];
        }
        else
            pinyinWordIndex = 0;
    }
    else {
        previousPinyinWord = pinyinWord;
        pinyinWordIndex = 0;
        if (translator.pinyinWordToObjects(pinyinWord, tmpObjects))
            translationObject = tmpObjects[pinyinWordIndex];
    }

    chineseWordLineEdit->setText(translationObject.chineseWord);
    monotonePinyinWordLineEdit->setText("");
    russianWordLineEdit->setText("");
    hskLineEdit->setText(QString::number(translationObject.hskNumber));
    russianWordsLineEdit->setText(translationObject.russianWords);
    examplesLineEdit->setText(translationObject.examples);
}

void ChineseDictionary::quit()
{
    close();
}

void ChineseDictionary::russianToChinese()
{
    TranslationObject emptyObject;
    emptyObject.hskNumber = 0;
    translationObject = emptyObject;

    QString russianWord = russianWordLineEdit->text();

    if (russianWord == previousRussianWord) {
        russianWordIndex++;
        if (!tmpObjects.empty()) {
            russianWordIndex %= tmpObjects.size();
            translationObject = tmpObjects[russianWordIndex];
        }
        else
            russianWordIndex = 0;
    }
    else {
        previousRussianWord = russianWord;
        russianWordIndex = 0;
        if (translator.russianWordToObjects(russianWord, tmpObjects))
            translationObject = tmpObjects[russianWordIndex];
    }

    chineseWordLineEdit->setText(translationObject.chineseWord);
    pinyinWordLineEdit->setText(translationObject.pinyinWord);
    hskLineEdit->setText(QString::number(translationObject.hskNumber));
    russianWordsLineEdit->setText(translationObject.russianWords);
    examplesLineEdit->setText(translationObject.examples);
}

/*

bool isControl = key == Qt::Key_Escape || key == Qt::Key_Left || key == Qt::Key_Right;
    bool isDigit = character >= QString("1") && character <= QString("9");
    bool isLetter = character >= QString("a") && character <= QString("z");

    if (!isControl && !isDigit && !isLetter)
        return false;

    if (isLetter) {
        monotonePinyin += keyEvent->text();
        selectorLineEdit->setText(translator.symbols(monotonePinyin, group));
    }
*/
