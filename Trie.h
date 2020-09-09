#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "../Global/Global.h"

namespace atheris
{
    class Trie
    {

        public:

            Trie()
            {
                root = std::make_shared<Node>();
            }

            ~Trie()
            {
                std::unique_lock<std::mutex> lock(m_mutex);
            }

            void insert(std::string& inWord);
            void insert(std::string&& inWord);

            void remove(std::string& inWord);
            void remove(std::string&& inWord);

            bool search(std::string& inWord);
            bool search(std::string&& inWord);

            unsigned searchCount(std::string& inWord);
            unsigned searchCount(std::string&& inWord);


        private:

            std::mutex m_mutex;

            struct Node
            {
                Node() 
                {

                    m_count = 0;
                    m_isWord = false;
                }

                std::unordered_map<char, std::shared_ptr<Node>> charMap;
                bool m_isWord;
                unsigned m_count;

                Node(const Node& node2) { charMap = node2.charMap; m_isWord = node2.m_isWord; m_count = node2.m_count; }
            };

            std::shared_ptr<Node> root;

            std::string toLower(const std::string& word)
            {
                std::string retString;
                for (const char& c : word)
                    retString += std::tolower(c);
                return retString;
            }

            bool hasChildren(const std::shared_ptr<Node>& node)
            {
                return !node->charMap.empty();
            }

            std::shared_ptr<Node> searchInternal(std::string&& inWord);
            void removeInternal(std::string&& inWord);
            void insertInternal(std::string&& inWord);
    };
}