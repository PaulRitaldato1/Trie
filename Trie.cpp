#include "Trie.h"
#include <stack>

namespace atheris
{
    void Trie::insert(std::string& inWord)
    {
	insertInternal(toLower(inWord));
    }

    void Trie::insert(std::string&& inWord)
    {
        insertInternal(toLower(inWord));
    }

    void Trie::insertInternal(std::string&& inWord)
    {

	std::shared_ptr<Node> temp = root;

	std::unique_lock<std::mutex> lock(m_mutex);
	for (const char key : inWord)
	{

		//check if char is in current node, if not, insert it
		if (temp->charMap.find(key) == temp->charMap.end())
		{
			temp->charMap.emplace(key, std::make_shared<Node>());
		}

		temp = temp->charMap[key];
	}

	++temp->m_count;
	temp->m_isWord = true;
    }

    void Trie::remove(std::string&& inWord)
    {

        if (!root)
        {
            return;
        }

        removeInternal(toLower(inWord));
    }

    void Trie::remove(std::string& inWord)
    {

	if (!root)
	{
		return;
	}

	removeInternal(toLower(inWord));
    }

    void Trie::removeInternal(std::string&& inWord)
    {

        std::shared_ptr<Node> temp = root;

        //get all the nodes belonging to a word
        std::stack<std::pair<std::shared_ptr<Node>, char>> wordStack;
        
        //lock before getting the current state of the Trie
        std::unique_lock<std::mutex> lock(m_mutex);

        for (const char key : inWord)
        {
            wordStack.push(std::make_pair(temp->charMap[key], key));
            temp = temp->charMap[key];
        }

        while (!wordStack.empty())
        {
            if (!hasChildren(wordStack.top().first))
            {
                if (wordStack.top().first->m_count > 1)
                {
                    --wordStack.top().first->m_count;
                    return;
                }
                else
                {
                    const char key = wordStack.top().second;
                    wordStack.pop();
                    if (wordStack.empty())
                        root->charMap.erase(key);
                    else
                        wordStack.top().first->charMap.erase(key);
                }
            }
            else if (wordStack.size() == inWord.size())
            {
                if (wordStack.top().first->m_count > 1)
                {
                    --wordStack.top().first->m_count;
                }
                else
                {
                    wordStack.top().first->m_isWord = false;
                    --wordStack.top().first->m_count;
                }
                return;
            }
            else
            {
                return;
            }

        }
    }

    bool Trie::search(std::string& inWord)
    {

	std::unique_lock<std::mutex> lock(m_mutex);

        auto temp = searchInternal(toLower(inWord));
        return temp ? temp->m_isWord : false;
    }

    bool Trie::search(std::string&& inWord)
    {

	std::unique_lock<std::mutex> lock(m_mutex);

        auto temp = searchInternal(toLower(inWord));
        return temp ? temp->m_isWord : false;
    }

    unsigned Trie::searchCount(std::string& inWord)
    {

	std::unique_lock<std::mutex> lock(m_mutex);

        auto temp = searchInternal(toLower(inWord));
        return temp ? temp->m_count : 0;
    }
    unsigned Trie::searchCount(std::string&& inWord)
    {

	std::unique_lock<std::mutex> lock(m_mutex);

        auto temp = searchInternal(toLower(inWord));
        return temp ? temp->m_count : 0;
    }

    std::shared_ptr<Trie::Node> Trie::searchInternal(std::string&& inWord)
    {

        std::shared_ptr<Node> temp = root;
        for (const char key : inWord)
        {
            if (temp->charMap.find(key) == temp->charMap.end())
            {
                return nullptr;
            }
            temp = temp->charMap[key];
        }

        return temp;
    }
}
