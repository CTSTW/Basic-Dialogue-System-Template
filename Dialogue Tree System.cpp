#include <iostream>
#include <vector>
#include <map>

struct DialogueNode {
    int id;
    std::string text;
    std::vector<std::pair<std::string, int>> choices;  // 选项文本和目标节点ID
    bool isEnd;
    
    DialogueNode(int id, std::string text, bool isEnd = false) 
        : id(id), text(text), isEnd(isEnd) {}
    
    void addChoice(std::string choiceText, int targetId) {
        choices.push_back({choiceText, targetId});
    }
};

class DialogueTree {
private:
    std::map<int, DialogueNode> nodes;
    int currentId;
    
public:
    DialogueTree() : currentId(0) {
        initializeDialogue();
    }
    
    void initializeDialogue() {
        // 创建对话树
        DialogueNode start(0, "喵呜~ 你是来找我玩的吗？");
        start.addChoice("当然啦！我们一起玩吧~", 1);
        start.addChoice("我是来给你送小鱼干的", 2);
        start.addChoice("我只是路过...", 3);
        nodes[0] = start;
        
        DialogueNode node1(1, "太好了！（开心地转圈圈）你想玩什么？");
        node1.addChoice("玩毛线球", 4);
        node1.addChoice("玩捉迷藏", 5);
        nodes[1] = node1;
        
        DialogueNode node2(2, "小鱼干！（眼睛闪闪发亮）谢谢你！");
        node2.addChoice("不客气，慢慢吃", 6);
        node2.addChoice("吃了要陪我玩哦", 7);
        nodes[2] = node2;
        
        // 结束节点
        DialogueNode end1(6, "（满足地吃着）喵呜~ 太好吃了！", true);
        nodes[6] = end1;
        
        DialogueNode end2(7, "嗯嗯！吃完就陪你玩！", true);
        nodes[7] = end2;
    }
    
    void startDialogue() {
        currentId = 0;
        
        while (true) {
            DialogueNode& currentNode = nodes[currentId];
            
            std::cout << "\n" << currentNode.text << std::endl;
            
            if (currentNode.isEnd) {
                std::cout << "\n[对话结束]\n";
                break;
            }
            
            // 显示选项
            for (int i = 0; i < currentNode.choices.size(); ++i) {
                std::cout << i + 1 << ". " << currentNode.choices[i].first << std::endl;
            }
            
            // 获取玩家选择
            int choice;
            std::cout << "请选择 (1-" << currentNode.choices.size() << "): ";
            std::cin >> choice;
            
            if (choice < 1 || choice > currentNode.choices.size()) {
                std::cout << "无效的选择！" << std::endl;
                continue;
            }
            
            currentId = currentNode.choices[choice - 1].second;
        }
    }
};

int main() {
    DialogueTree tree;
    tree.startDialogue();
    return 0;
}