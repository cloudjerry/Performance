
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <list>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class Block
{
public:
    Block() { }
    ~Block() { }

    std::string GetName() { return m_name; }
    void SetName(std::string name) { m_name = name; }

	void AddAttribute(std::string attribute) { m_attributes.insert(attribute); }
	void SetAttributes(std::set<std::string> attributes){m_attributes = attributes;}
    std::set<std::string> GetAttributes() { return m_attributes; }


private:
    std::string m_name;
	std::set<std::string> m_attributes;
};

class MapGraph
{

public:

    void AddBlock(Block* block)
    {
        std::string uniqueName = GetUniqueBlockNameInGraph(block);
        block->SetName(uniqueName);

		m_blocks.insert(make_pair(uniqueName, block->GetAttributes()));
    }

    std::set<Block*> GetBlocksWithAttribute(std::string attribute)
    {
        std::set<Block*> blocksWithAttribute;
		std::map<std::string, std::set<std::string>>::iterator it;
		for(it = m_blocks.begin(); it != m_blocks.end(); it++)
        {
            std::set<std::string> attributes = it->second;

			if(attributes.find(attribute) != attributes.end())
			{
				Block *b = new Block; 
				b->SetName(it->first);
				b->SetAttributes(attributes);
                blocksWithAttribute.insert(b);
                break;
			}

        }
        return blocksWithAttribute;
    }

private:

    bool BlockNameExistsInGraph(std::string blockName)
    {
        bool exists = false;
		std::map<std::string, std::set<std::string>>::iterator it = m_blocks.find(blockName);
		if (it != m_blocks.end())
		{
			exists = true;
		}

        return exists;
    }

    std::string GetUniqueBlockNameInGraph(Block* block)
    {
        std::string originalName = block->GetName();
        std::string potentialName = block->GetName();
        unsigned int count = 0;

        bool foundUniqueName = false;
        while (foundUniqueName == false)
        {
            if (!BlockNameExistsInGraph(potentialName))
            {
                foundUniqueName = true;
            }
			else
            {
                std::ostringstream s;
                s << originalName << count;
                potentialName = s.str();
                count++;
            }
        }

        return potentialName;
    }

private:
	std::map<std::string, std::set<std::string>> m_blocks;

};


// Testing setup 

static const int s_blockQuantity = 500;
static const int s_attributesPerBlock = 100;
static std::string s_blockBaseName("map");

std::vector<std::vector<std::string>> GetAttributeData()
{
    std::vector<std::vector<std::string>> attrData;

    for (int i = 0; i < s_blockQuantity; ++i)
    {
        std::vector<std::string> blockAttr;

        for (int j = 0; j < s_attributesPerBlock; j++)
        {
            std::ostringstream s;
            s << "attribute_" << j + i;
            blockAttr.push_back(s.str());
        }

        attrData.push_back(blockAttr);
    }

    return attrData;
}

std::chrono::milliseconds Measure_AddBlocks(MapGraph& graph)
{
    auto attrData = GetAttributeData();

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < s_blockQuantity; ++i)
    {
        Block* b = new Block;
        b->SetName(s_blockBaseName);
        for (auto a: attrData[i])
        {
            b->AddAttribute(a);
        }
        graph.AddBlock(b);
    }
    auto end = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds Measure_FindBlocksByAttribute(MapGraph& graph)
{
    auto start = std::chrono::system_clock::now();
    for (int j = 0; j < s_blockQuantity * 2; j++)
    {
        std::ostringstream s;
        s << "attribute_" << j;
        graph.GetBlocksWithAttribute(s.str()).size();
    }
    auto end = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

int _tmain(int argc, _TCHAR* argv[])
{
    MapGraph graph;

    auto timeAddBlocks = Measure_AddBlocks(graph);
    std::cout << "Measure_AddBlocks took: " << timeAddBlocks.count() << "ms.\n";

    auto timeFindBocksByAttribute = Measure_FindBlocksByAttribute(graph);
    std::cout << "Measure_FindBlocksByAttribute took: " << timeFindBocksByAttribute.count() << "ms.\n";

    return 0;
}


