#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>

class CuteCharacter {
private:
    std::string name;
    std::string personality;  // 性格特点
    int mood;  // 心情值 0-100
    
    // 对话库
    std::map<std::string, std::vector<std::string>> dialogues;
    
public:
    CuteCharacter(std::string name, std::string personality) 
        : name(name), personality(personality), mood(80) {}
    
    // 添加对话选项
    void addDialogue(std::string category, std::vector<std::string> lines) {
        dialogues[category] = lines;
    }
    
    // 根据心情和情境获取对话
    std::string speak(std::string situation) {
        if (dialogues.find(situation) == dialogues.end()) {
            return getRandomDefaultDialogue();
        }
        
        std::vector<std::string>& options = dialogues[situation];
        
        // 根据心情选择不同的回应
        int index;
        if (mood > 70) {  // 心情很好
            index = 0;  // 最积极的回应
        } else if (mood > 40) {  // 心情一般
            index = 1 % options.size();
        } else {  // 心情不好
            index = (options.size() - 1) % options.size();  // 较消极的回应
        }
        
        return options[index];
    }
    
    // 改变心情
    void changeMood(int delta) {
        mood += delta;
        if (mood < 0) mood = 0;
        if (mood > 100) mood = 100;
    }
    
    // 获取随机默认对话
    std::string getRandomDefaultDialogue() {
        std::vector<std::string> defaults = {
            "喵~ 我不太明白呢~",
            "唔... 这个要怎么回答呢？",
            "（歪着头思考）",
            "你能再说一遍吗？>_<"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, defaults.size() - 1);
        
        return defaults[dis(gen)];
    }
    
    // 获取角色信息
    void displayInfo() {
        std::cout << "✨ " << name << " ✨" << std::endl;
        std::cout << "性格: " << personality << std::endl;
        std::cout << "心情: " << getMoodEmoji() << " (" << mood << "/100)" << std::endl;
    }
    
private:
    std::string getMoodEmoji() {
        if (mood > 80) return "😊";
        if (mood > 60) return "🙂";
        if (mood > 40) return "😐";
        if (mood > 20) return "😕";
        return "😢";
    }
};

// 对话管理器
class DialogueManager {
private:
    CuteCharacter* character;
    
public:
    DialogueManager(CuteCharacter* character) : character(character) {}
    
    // 开始对话
    void startConversation() {
        std::cout << "\n=== 对话开始 ===\n";
        character->displayInfo();
        
        std::string input;
        while (true) {
            std::cout << "\n输入对话主题（或输入'bye'退出）: ";
            std::getline(std::cin, input);
            
            if (input == "bye") {
                std::cout << character->speak("farewell") << std::endl;
                break;
            }
            
            std::string response = character->speak(input);
            std::cout << character->speak(input) << std::endl;
            
            // 根据对话内容改变心情
            updateMood(input);
        }
    }
    
private:
    void updateMood(const std::string& topic) {
        // 简单的情绪反应规则
        if (topic.find("开心") != std::string::npos || 
            topic.find("喜欢") != std::string::npos) {
            character->changeMood(10);
        } else if (topic.find("生气") != std::string::npos || 
                   topic.find("讨厌") != std::string::npos) {
            character->changeMood(-10);
        }
    }
};

// 创建可爱角色
CuteCharacter createCuteCat() {
    CuteCharacter cat("小猫咪", "傲娇又粘人");
    
    // 添加各种情境的对话
    cat.addDialogue("greeting", {
        "喵呜~ 你来啦！我好想你呀~",
        "（蹭蹭你的手）今天有带小鱼干吗？",
        "哼！怎么现在才来！我都等了好久啦！"
    });
    
    cat.addDialogue("weather", {
        "今天天气真好呢，适合晒太阳~",
        "外面在下雨，我有点怕打雷...",
        "喵~ 我想出去玩！"
    });
    
    cat.addDialogue("food", {
        "小鱼干！小鱼干！最爱小鱼干了！",
        "（眼睛发光）有零食吃吗？",
        "唔... 有点饿了呢..."
    });
    
    cat.addDialogue("play", {
        "要来玩毛线球吗？超有趣的！",
        "（兴奋地摇尾巴）",
        "陪我玩嘛~ 不要不理我嘛~"
    });
    
    cat.addDialogue("farewell", {
        "这么快就要走了吗？我会想你的...",
        "喵！记得明天还要来看我哦！",
        "（挥手）再见啦~"
    });
    
    return cat;
}

int main() {
    // 创建角色
    CuteCharacter cat = createCuteCat();
    
    // 创建对话管理器
    DialogueManager manager(&cat);
    
    // 开始对话
    manager.startConversation();
    
    return 0;
}