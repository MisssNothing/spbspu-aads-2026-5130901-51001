#ifndef DICTDISPATCHER_HPP
#define DICTDISPATCHER_HPP

#include "dictionary.hpp"
#include "../common/list.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace petrenko {

struct DictStorage
{
  std::string name_;
  Dictionary dict_;
};

class DictDispatcher
{
private:
  List<DictStorage> dictionaries_;

  int findDictionary(const std::string& name) const
  {
    int index = 0;
    for (LCIter<DictStorage> it = dictionaries_.cbegin();
         it != dictionaries_.cend(); ++it)
    {
      if ((*it).name_ == name)
      {
        return index;
      }
      ++index;
    }
    return -1;
  }

  bool dictExists(const std::string& name) const
  {
    return findDictionary(name) != -1;
  }

  Dictionary& getDictionary(const std::string& name)
  {
    int idx = findDictionary(name);
    if (idx == -1)
    {
      throw std::string("Словарь не найден");
    }

    int index = 0;
    for (LIter<DictStorage> it = dictionaries_.begin();
         it != dictionaries_.end(); ++it)
    {
      if (index == idx)
      {
        return (*it).dict_;
      }
      ++index;
    }
    throw std::string("Словарь не найден");
  }

  const Dictionary& getDictionaryConst(const std::string& name) const
  {
    int idx = findDictionary(name);
    if (idx == -1)
    {
      throw std::string("Словарь не найден");
    }

    int index = 0;
    for (LCIter<DictStorage> it = dictionaries_.cbegin();
         it != dictionaries_.cend(); ++it)
    {
      if (index == idx)
      {
        return (*it).dict_;
      }
      ++index;
    }
    throw std::string("Словарь не найден");
  }

  void createDictionary(const std::string& name)
  {
    if (dictExists(name))
    {
      throw std::string("Словарь уже существует");
    }

    DictStorage storage;
    storage.name_ = name;
    dictionaries_.insert(storage, dictionaries_.getSize());
  }

  void dropDictionary(const std::string& name)
  {
    int idx = findDictionary(name);
    if (idx == -1)
    {
      throw std::string("Словарь не найден");
    }

    dictionaries_.removeAt(idx);
  }

  std::string toLowercase(const std::string& str) const
  {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

  std::string removePunctuation(const std::string& word) const
  {
    std::string result;
    for (size_t i = 0; i < word.length(); ++i)
    {
      char c = word[i];
      if (std::isalpha(c) || c == '\'')
      {
        result += c;
      }
    }
    return result;
  }

public:
  void processCommand(const std::string& line)
  {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    try
    {
      if (cmd == "make")
      {
        std::string dictName;
        ss >> dictName;

        if (dictName.empty())
        {
          throw std::string("Отсутствует название словаря");
        }

        createDictionary(dictName);
        std::cout << "OK: словарь " << dictName << " создан" << '\n';
      }
      else if (cmd == "drop")
      {
        std::string dictName;
        ss >> dictName;

        if (dictName.empty())
        {
          throw std::string("Отсутствует название словаря");
        }

        dropDictionary(dictName);
        std::cout << "OK: словарь " << dictName << " удален" << '\n';
      }
      else if (cmd == "add")
      {
        std::string dictName, word, translation, pos;
        ss >> dictName >> word >> translation >> pos;

        if (dictName.empty() || word.empty() || translation.empty() || pos.empty())
        {
          throw std::string("Параметры отсутствуют ");
        }

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        WordEntry existingEntry = getDictionaryConst(dictName).getWordEntry(word);
        if (existingEntry.hasTranslations())
        {
          PosNode* unknownNode = existingEntry.findPos("unknown");
          if (unknownNode && unknownNode->translations_)
          {
            getDictionary(dictName).removeTranslation(word, "(требуется перевод)", "unknown");
          }
        }

        getDictionary(dictName).addTranslation(word, translation, pos);
        std::cout << "OK: добавлен перевод " << word << " (" << pos
                << ") -> " << translation << '\n';
      }
      else if (cmd == "removeTranslation")
      {
        std::string dictName, word, translation, pos;
        ss >> dictName >> word >> translation >> pos;

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        if (getDictionary(dictName).removeTranslation(word, translation, pos))
        {
          std::cout << word << ": удалён " << pos << " " << translation
                    << '\n';
        }
        else
        {
          throw std::string("Перевод не найден");
        }
      }
      else if (cmd == "show")
      {
        std::string dictName;
        ss >> dictName;

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        std::string word;
        if (ss >> word)
        {
          WordEntry entry = getDictionaryConst(dictName).getWordEntry(word);

          if (!entry.hasTranslations())
          {
            std::cout << "Слово '" << word << "' не найдено" << '\n';
          }
          else
          {
            std::string lowerWord = toLowercase(word);
            std::cout << lowerWord << ":" << '\n';

            PosNode* posCurr = entry.posList_;
            while (posCurr)
            {
              std::cout << "  " << posCurr->pos_ << ": ";

              TranslationNode* transCurr = posCurr->translations_;
              bool first = true;

              while (transCurr)
              {
                if (!first)
                {
                  std::cout << ", ";
                }
                std::cout << transCurr->translation_;
                first = false;
                transCurr = transCurr->next_;
              }

              std::cout << '\n';
              posCurr = posCurr->next_;
            }
          }
        }
        else
        {
          std::cout << dictName << ":" << '\n';
          std::vector<std::string> words = getDictionaryConst(dictName).getAllWords();

          for (size_t i = 0; i < words.size(); ++i)
          {
            const std::string& w = words[i];
            WordEntry entry = getDictionaryConst(dictName).getWordEntry(w);

            std::cout << "  " << w << ":" << '\n';

            PosNode* posCurr = entry.posList_;
            while (posCurr)
            {
              std::cout << "    " << posCurr->pos_ << ": ";

              TranslationNode* transCurr = posCurr->translations_;
              bool first = true;

              while (transCurr)
              {
                if (!first)
                {
                  std::cout << ", ";
                }
                std::cout << transCurr->translation_;
                first = false;
                transCurr = transCurr->next_;
              }

              std::cout << '\n';
              posCurr = posCurr->next_;
            }
          }
        }
      }
      else if (cmd == "findPos")
      {
        std::string dictName, pos;
        ss >> dictName >> pos;

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        std::cout << pos << ":" << '\n';
        std::vector<std::string> words = getDictionaryConst(dictName).findWordsByPos(pos);

        for (size_t i = 0; i < words.size(); ++i)
        {
          const std::string& word = words[i];
          WordEntry entry = getDictionaryConst(dictName).getWordEntry(word);
          PosNode* posNode = entry.findPos(pos);

          if (posNode && posNode->translations_)
          {
            std::cout << "  " << word << ": ";

            TranslationNode* transCurr = posNode->translations_;
            bool first = true;

            while (transCurr)
            {
              if (!first)
              {
                std::cout << ", ";
              }
              std::cout << transCurr->translation_;
              first = false;
              transCurr = transCurr->next_;
            }

            std::cout << '\n';
          }
        }
      }
      else if (cmd == "findTranslation")
      {
        std::string dictName, russianWord;
        ss >> dictName >> russianWord;

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        std::cout << russianWord << ":" << '\n';
        std::vector<Dictionary::RussianSearchResult> results =
          getDictionaryConst(dictName).findRussianTranslation(russianWord);

        for (size_t i = 0; i < results.size(); ++i)
        {
          std::cout << "  " << results[i].word_ << ":" << '\n';
          std::cout << "    " << results[i].pos_ << ": ";

          for (size_t j = 0; j < results[i].translations_.size(); ++j)
          {
            if (j > 0)
            {
              std::cout << ", ";
            }
            std::cout << results[i].translations_[j];
          }

          std::cout << '\n';
        }

        if (results.empty())
        {
          std::cout << "  перевод не найден" << '\n';
        }
      }
      else if (cmd == "merge")
      {
        std::string newDict, dict1, dict2;
        ss >> newDict >> dict1 >> dict2;

        if (!dictExists(dict1) || !dictExists(dict2))
        {
          throw std::string("Исходные словари не найдены");
        }

        if (dictExists(newDict))
        {
          throw std::string("Target Словарь уже существует");
        }

        createDictionary(newDict);
        Dictionary& target = getDictionary(newDict);
        target.merge(getDictionaryConst(dict1));
        target.merge(getDictionaryConst(dict2));

        std::cout << "OK: словарь " << newDict << " создан" << '\n';
      }
      else if (cmd == "export")
      {
        std::string dictName, filename;
        ss >> dictName >> filename;

        if (!dictExists(dictName))
        {
          throw std::string("Словарь не найден");
        }

        std::ofstream file(filename.c_str());

        if (!file.is_open())
        {
          throw std::string("Cannot create file");
        }

        std::vector<Dictionary::ExportEntry> entries =
          getDictionaryConst(dictName).getAllTranslations();

        for (size_t i = 0; i < entries.size(); ++i)
        {
          file << entries[i].word_ << "|" << entries[i].pos_
               << "|" << entries[i].translation_ << '\n';
        }

        file.close();
        std::cout << "OK: словарь " << dictName << " экспортирован" << '\n';
      }
      else if (cmd == "exportTranslation")
      {
        std::string inputFile, sourceDict, outputFile, unknownDict;
        ss >> inputFile >> sourceDict >> outputFile >> unknownDict;

        if (inputFile.empty() || sourceDict.empty() || outputFile.empty() || unknownDict.empty())
        {
          throw std::string("Сигнатура: exportTranslation <input-file> <source-dict> <output-file> <unknown-dict>");
        }

        if (!dictExists(sourceDict))
        {
          throw std::string("Исходный словарь не найден");
        }

        if (!dictExists(unknownDict))
        {
          createDictionary(unknownDict);
        }
        else
        {
          dropDictionary(unknownDict);
          createDictionary(unknownDict);
        }

        std::ifstream infile(inputFile.c_str());

        if (!infile.is_open())
        {
          throw std::string("Не удается открыть входной файл");
        }

        std::ofstream outfile(outputFile.c_str());

        if (!outfile.is_open())
        {
          infile.close();
          throw std::string("Не удается создать выходной файл");
        }

        std::string line;
        std::vector<std::string> unknownWordsList;

        while (std::getline(infile, line))
        {
          std::stringstream wordsStream(line);
          std::string englishWord;
          bool firstWord = true;

          while (wordsStream >> englishWord)
          {
            std::string cleanWord = removePunctuation(englishWord);
            if (cleanWord.empty())
            {
              if (!firstWord)
              {
                outfile << " ";
              }
              outfile << englishWord;
              firstWord = false;
              continue;
            }

            std::string lowerWord = toLowercase(cleanWord);
            WordEntry entry = getDictionaryConst(sourceDict).getWordEntry(lowerWord);

            if (entry.hasTranslations())
            {
              std::string bestTranslation;
              PosNode* posCurr = entry.posList_;

              while (posCurr)
              {
                if (posCurr->translations_)
                {
                  bestTranslation = posCurr->translations_->translation_;
                  break;
                }
                posCurr = posCurr->next_;
              }

              if (!firstWord)
              {
                outfile << " ";
              }
              outfile << bestTranslation;
            }
            else
            {
              bool alreadyAdded = false;
              for (size_t i = 0; i < unknownWordsList.size(); ++i)
              {
                if (unknownWordsList[i] == englishWord)
                {
                  alreadyAdded = true;
                  break;
                }
              }

              if (!alreadyAdded)
              {
                unknownWordsList.push_back(englishWord);
                getDictionary(unknownDict).addTranslation(englishWord, "(требуется перевод)", "unknown");
              }

              if (!firstWord)
              {
                outfile << " ";
              }
              outfile << englishWord;
            }

            firstWord = false;
          }

          outfile << '\n';
        }

        infile.close();
        outfile.close();

        std::cout << "OK: перевод создан" << '\n';

        if (unknownWordsList.size() > 0)
        {
          std::cout << "Неизвестные слова (" << unknownWordsList.size()
                    << ") сохранены в словарь " << unknownDict << '\n';
        }
      }
      else if (cmd == "intersection")
      {
        std::string newDict, dict1, dict2;
        ss >> newDict >> dict1 >> dict2;

        if (!dictExists(dict1) || !dictExists(dict2))
        {
          throw std::string("Исходные словари не найдены");
        }

        if (dictExists(newDict))
        {
          throw std::string("Target Словарь уже существует");
        }

        createDictionary(newDict);
        Dictionary& target = getDictionary(newDict);

        std::vector<std::string> words1 = getDictionaryConst(dict1).getAllWords();

        for (size_t i = 0; i < words1.size(); ++i)
        {
          const std::string& word = words1[i];

          if (getDictionaryConst(dict2).hasWord(word))
          {
            WordEntry entry = getDictionaryConst(dict1).getWordEntry(word);

            PosNode* posCurr = entry.posList_;
            while (posCurr)
            {
              TranslationNode* transCurr = posCurr->translations_;
              while (transCurr)
              {
                target.addTranslation(
                  word, transCurr->translation_, posCurr->pos_);
                transCurr = transCurr->next_;
              }
              posCurr = posCurr->next_;
            }
          }
        }

        std::cout << "OK: словарь " << newDict << " создан" << '\n';
      }
      else if (cmd == "symmDiff")
      {
        std::string newDict, dict1, dict2;
        ss >> newDict >> dict1 >> dict2;

        if (!dictExists(dict1) || !dictExists(dict2))
        {
          throw std::string("Исходные словари не найдены");
        }

        if (dictExists(newDict))
        {
          throw std::string("Target Словарь уже существует");
        }

        createDictionary(newDict);
        Dictionary& target = getDictionary(newDict);

        std::vector<std::string> words1 = getDictionaryConst(dict1).getAllWords();

        for (size_t i = 0; i < words1.size(); ++i)
        {
          const std::string& word = words1[i];

          if (!getDictionaryConst(dict2).hasWord(word))
          {
            WordEntry entry = getDictionaryConst(dict1).getWordEntry(word);

            PosNode* posCurr = entry.posList_;
            while (posCurr)
            {
              TranslationNode* transCurr = posCurr->translations_;
              while (transCurr)
              {
                target.addTranslation(
                  word, transCurr->translation_, posCurr->pos_);
                transCurr = transCurr->next_;
              }
              posCurr = posCurr->next_;
            }
          }
        }

        std::vector<std::string> words2 = getDictionaryConst(dict2).getAllWords();

        for (size_t i = 0; i < words2.size(); ++i)
        {
          const std::string& word = words2[i];

          if (!getDictionaryConst(dict1).hasWord(word))
          {
            WordEntry entry = getDictionaryConst(dict2).getWordEntry(word);

            PosNode* posCurr = entry.posList_;
            while (posCurr)
            {
              TranslationNode* transCurr = posCurr->translations_;
              while (transCurr)
              {
                target.addTranslation(
                  word, transCurr->translation_, posCurr->pos_);
                transCurr = transCurr->next_;
              }
              posCurr = posCurr->next_;
            }
          }
        }

        std::cout << "OK: словарь " << newDict << " создан" << '\n';
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
    }
    catch (const std::string& error)
    {
      std::cout << "<INVALID COMMAND> " << error << '\n';
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
  }
};

}

#endif
