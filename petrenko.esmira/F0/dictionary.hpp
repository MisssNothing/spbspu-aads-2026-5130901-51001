#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "avltree.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

namespace petrenko {

struct TranslationNode
{
  std::string translation_;
  TranslationNode* next_;

  TranslationNode(const std::string& translation)
    : translation_(translation)
    , next_(nullptr)
  {
  }
};

struct PosNode
{
  std::string pos_;
  TranslationNode* translations_;
  PosNode* next_;

  PosNode(const std::string& pos)
    : pos_(pos)
    , translations_(nullptr)
    , next_(nullptr)
  {
  }

  ~PosNode()
  {
    TranslationNode* curr = translations_;
    while (curr)
    {
      TranslationNode* temp = curr;
      curr = curr->next_;
      delete temp;
    }
  }
};

struct WordEntry
{
  PosNode* posList_;

  WordEntry()
    : posList_(nullptr)
  {
  }

  WordEntry(const WordEntry& other)
    : posList_(nullptr)
  {
    PosNode* curr = other.posList_;
    while (curr)
    {
      addPos(curr->pos_);

      TranslationNode* transCurr = curr->translations_;
      while (transCurr)
      {
        addTranslation(curr->pos_, transCurr->translation_);
        transCurr = transCurr->next_;
      }

      curr = curr->next_;
    }
  }

  ~WordEntry()
  {
    clear();
  }

  WordEntry& operator=(const WordEntry& other)
  {
    if (this != &other)
    {
      clear();

      PosNode* curr = other.posList_;
      while (curr)
      {
        addPos(curr->pos_);

        TranslationNode* transCurr = curr->translations_;
        while (transCurr)
        {
          addTranslation(curr->pos_, transCurr->translation_);
          transCurr = transCurr->next_;
        }

        curr = curr->next_;
      }
    }
    return *this;
  }

  void clear()
  {
    PosNode* curr = posList_;
    while (curr)
    {
      PosNode* temp = curr;
      curr = curr->next_;
      delete temp;
    }
    posList_ = nullptr;
  }

  void addPos(const std::string& pos)
  {
    if (!posList_)
    {
      posList_ = new PosNode(pos);
      return;
    }

    PosNode* curr = posList_;
    while (curr->next_)
    {
      if (curr->pos_ == pos)
      {
        return;
      }
      curr = curr->next_;
    }

    if (curr->pos_ != pos)
    {
      curr->next_ = new PosNode(pos);
    }
  }

  void removePos(const std::string& pos)
  {
    if (!posList_)
    {
      return;
    }

    if (posList_->pos_ == pos)
    {
      PosNode* temp = posList_;
      posList_ = posList_->next_;
      delete temp;
      return;
    }

    PosNode* curr = posList_;
    while (curr->next_)
    {
      if (curr->next_->pos_ == pos)
      {
        PosNode* temp = curr->next_;
        curr->next_ = temp->next_;
        delete temp;
        return;
      }
      curr = curr->next_;
    }
  }

  PosNode* findPos(const std::string& pos) const
  {
    PosNode* curr = posList_;
    while (curr)
    {
      if (curr->pos_ == pos)
      {
        return curr;
      }
      curr = curr->next_;
    }
    return nullptr;
  }

  void addTranslation(const std::string& pos, const std::string& translation)
  {
    PosNode* posNode = findPos(pos);
    if (!posNode)
    {
      addPos(pos);
      posNode = findPos(pos);
    }

    TranslationNode* curr = posNode->translations_;
    while (curr)
    {
      if (curr->translation_ == translation)
      {
        return;
      }
      curr = curr->next_;
    }

    TranslationNode* newTrans = new TranslationNode(translation);
    newTrans->next_ = posNode->translations_;
    posNode->translations_ = newTrans;
  }

  bool removeTranslation(const std::string& pos, const std::string& translation)
  {
    PosNode* posNode = findPos(pos);
    if (!posNode)
    {
      return false;
    }

    TranslationNode* curr = posNode->translations_;
    TranslationNode* prev = nullptr;

    while (curr)
    {
      if (curr->translation_ == translation)
      {
        if (prev)
        {
          prev->next_ = curr->next_;
        }
        else
        {
          posNode->translations_ = curr->next_;
        }
        delete curr;
        return true;
      }
      prev = curr;
      curr = curr->next_;
    }

    return false;
  }

  bool hasTranslations() const
  {
    if (!posList_)
    {
      return false;
    }

    PosNode* curr = posList_;
    while (curr)
    {
      if (curr->translations_)
      {
        return true;
      }
      curr = curr->next_;
    }
    return false;
  }
};

class Dictionary
{
private:
  AVLTree<std::string, WordEntry> words_;

  std::string toLowercase(const std::string& str) const
  {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

public:
  Dictionary() = default;

  Dictionary(const Dictionary& other)
    : words_(other.words_)
  {
  }

  Dictionary& operator=(const Dictionary& other)
  {
    if (this != &other)
    {
      words_ = other.words_;
    }
    return *this;
  }

  ~Dictionary() = default;

  void addTranslation(const std::string& word,
                      const std::string& translation,
                      const std::string& pos)
  {
    std::string lowerWord = toLowercase(word);
    WordEntry entry;

    if (words_.find(lowerWord, entry))
    {
      entry.addTranslation(pos, translation);
      words_.insert(lowerWord, entry);
    }
    else
    {
      entry.addTranslation(pos, translation);
      words_.insert(lowerWord, entry);
    }
  }

  bool removeTranslation(const std::string& word,
                         const std::string& translation,
                         const std::string& pos)
  {
    std::string lowerWord = toLowercase(word);
    WordEntry entry;

    if (!words_.find(lowerWord, entry))
    {
      return false;
    }

    if (entry.removeTranslation(pos, translation))
    {
      PosNode* posNode = entry.findPos(pos);
      if (posNode && !posNode->translations_)
      {
        entry.removePos(pos);
      }

      if (entry.hasTranslations())
      {
        words_.insert(lowerWord, entry);
      }
      else
      {
        words_.remove(lowerWord);
      }
      return true;
    }
    return false;
  }

  WordEntry getWordEntry(const std::string& word) const
  {
    std::string lowerWord = toLowercase(word);
    WordEntry entry;
    words_.find(lowerWord, entry);
    return entry;
  }

  std::vector<std::string> getAllWords() const
  {
    std::vector<std::pair<std::string, WordEntry>> pairs = words_.toVector();
    std::vector<std::string> result;

    for (size_t i = 0; i < pairs.size(); ++i)
    {
      result.push_back(pairs[i].first);
    }

    return result;
  }

  bool hasWord(const std::string& word) const
  {
    std::string lowerWord = toLowercase(word);
    return words_.contains(lowerWord);
  }

  void merge(const Dictionary& other)
  {
    std::vector<std::pair<std::string, WordEntry>> otherWords =
      other.words_.toVector();

    for (size_t i = 0; i < otherWords.size(); ++i)
    {
      const std::string& word = otherWords[i].first;
      const WordEntry& otherEntry = otherWords[i].second;

      WordEntry myEntry;
      words_.find(word, myEntry);

      PosNode* posCurr = otherEntry.posList_;
      while (posCurr)
      {
        TranslationNode* transCurr = posCurr->translations_;
        while (transCurr)
        {
          myEntry.addTranslation(posCurr->pos_, transCurr->translation_);
          transCurr = transCurr->next_;
        }
        posCurr = posCurr->next_;
      }

      words_.insert(word, myEntry);
    }
  }

  std::vector<std::string> findWordsByPos(const std::string& pos) const
  {
    std::vector<std::string> result;
    std::vector<std::pair<std::string, WordEntry>> allWords = words_.toVector();

    for (size_t i = 0; i < allWords.size(); ++i)
    {
      const std::string& word = allWords[i].first;
      const WordEntry& entry = allWords[i].second;

      PosNode* posNode = entry.findPos(pos);
      if (posNode && posNode->translations_)
      {
        result.push_back(word);
      }
    }

    return result;
  }

  struct RussianSearchResult
  {
    std::string word_;
    std::string pos_;
    std::vector<std::string> translations_;
  };

  std::vector<RussianSearchResult> findRussianTranslation(
    const std::string& russianWord) const
  {
    std::vector<RussianSearchResult> result;
    std::vector<std::pair<std::string, WordEntry>> allWords = words_.toVector();

    for (size_t i = 0; i < allWords.size(); ++i)
    {
      const std::string& word = allWords[i].first;
      const WordEntry& entry = allWords[i].second;

      PosNode* posCurr = entry.posList_;
      while (posCurr)
      {
        TranslationNode* transCurr = posCurr->translations_;
        while (transCurr)
        {
          if (transCurr->translation_ == russianWord)
          {
            RussianSearchResult res;
            res.word_ = word;
            res.pos_ = posCurr->pos_;

            TranslationNode* tCurr = posCurr->translations_;
            while (tCurr)
            {
              res.translations_.push_back(tCurr->translation_);
              tCurr = tCurr->next_;
            }

            result.push_back(res);
            break;
          }
          transCurr = transCurr->next_;
        }
        posCurr = posCurr->next_;
      }
    }

    return result;
  }

  struct ExportEntry
  {
    std::string word_;
    std::string pos_;
    std::string translation_;
  };

  std::vector<ExportEntry> getAllTranslations() const
  {
    std::vector<ExportEntry> result;
    std::vector<std::pair<std::string, WordEntry>> allWords = words_.toVector();

    for (size_t i = 0; i < allWords.size(); ++i)
    {
      const std::string& word = allWords[i].first;
      const WordEntry& entry = allWords[i].second;

      PosNode* posCurr = entry.posList_;
      while (posCurr)
      {
        TranslationNode* transCurr = posCurr->translations_;
        while (transCurr)
        {
          ExportEntry exp;
          exp.word_ = word;
          exp.pos_ = posCurr->pos_;
          exp.translation_ = transCurr->translation_;
          result.push_back(exp);
          transCurr = transCurr->next_;
        }
        posCurr = posCurr->next_;
      }
    }

    return result;
  }
};

}

#endif
