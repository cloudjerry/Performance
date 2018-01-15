
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <list>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

class Block
{
public:
    Block() { }
    ~Block() { }

    std::string GetName() { return m_name; }
    void SetName(std::string name) { m_name = name; }

    void AddAttribute(std::string attribute) { m_attributes.push_front(attribute); }
    std::list<std::string> GetAttributes() { return m_attributes; }

private:
    std::string m_name;
    std::list<std::string> m_attributes;
};

class ListGraph
{

public:

    void AddBlock(Block* block)
    {

        std::string uniqueName = GetUniqueBlockNameInGraph(block);
        block->SetName(uniqueName);


        m_blocks.push_front(block);
    }

    std::list<Block*> GetBlocksWithAttribute(std::string attribute)
    {
        std::list<Block*> blocksWithAttribute;
        for (Block* b: m_blocks)
        {
            std::list<std::string> attributes = b->GetAttributes();
            for (std::string s : attributes)
            {
                if (s == attribute)
                {
                    blocksWithAttribute.push_front(b);
                    break;
                }
            }
        }
        return blocksWithAttribute;
    }

private:

    bool BlockNameExistsInGraph(std::string blockName)
    {
        bool exists = false;
        for (auto i = m_blocks.begin(); i != m_blocks.end(); i++)
        {
            Block* iterBlock = *i;
            std::string iterName = iterBlock->GetName();
            if (iterName == blockName)
            {
                exists = true;
            }
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
            else if (BlockNameExistsInGraph(potentialName))
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

    std::list<Block*> m_blocks;
};


// Testing

static const int s_blockQuantity = 500;
static const int s_attributesPerBlock = 100;
static std::string s_blockBaseName("basename");

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

std::chrono::milliseconds Measure_AddBlocks(ListGraph& graph)
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

std::chrono::milliseconds Measure_FindBlocksByAttribute(ListGraph& graph)
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
    ListGraph graph;

    auto timeAddBlocks = Measure_AddBlocks(graph);
    std::cout << "Measure_AddBlocks took: " << timeAddBlocks.count() << "ms.\n";

    auto timeFindBocksByAttribute = Measure_FindBlocksByAttribute(graph);
    std::cout << "Measure_FindBlocksByAttribute took: " << timeFindBocksByAttribute.count() << "ms.\n";

    return 0;
}
