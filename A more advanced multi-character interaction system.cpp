#include <iostream>
#include <vector>
#include <memory>

// 基础对话组件
class DialogueComponent {
public:
    virtual std::string getLine() = 0;
    virtual ~DialogueComponent() {}
};

// 可爱语气组件
class CuteToneComponent : public DialogueComponent {
private:
    std::vector<std::string> cuteSuffixes = {"~", "喵", "呢", "哦", "呀"};
    
public:
    std::string addCuteTone(const std::string& line) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, cuteSuffixes.size() - 1);
        
        return line + cuteSuffixes[dis(gen)];
    }
    
    std::string getLine() override {
        return "";
    }
};

// 具体角色
class KawaiiCharacter {
protected:
    std::string name;
    std::unique_ptr<CuteToneComponent> toneComponent;
    
public:
    KawaiiCharacter(const std::string& name) : name(name) {
        toneComponent = std::make_unique<CuteToneComponent>();
    }
    
    virtual std::string introduce() = 0;
    virtual std::string reactTo(const std::string& event) = 0;
    
    std::string speak(const std::string& text) {
        return name + ": \"" + toneComponent->addCuteTone(text) + "\"";
    }
};

// 示例角色：魔法少女
class MagicalGirl : public KawaiiCharacter {
public:
    MagicalGirl() : KawaiiCharacter("小樱") {}
    
    std::string introduce() override {
        return speak("我是魔法少女小樱！爱与正义的使者！");
    }
    
    std::string reactTo(const std::string& event) override {
        if (event == "战斗") {
            return speak("以月亮的名义，我要消灭你！");
        } else if (event == "美食") {
            return speak("草莓蛋糕！我最喜欢了！");
        }
        return speak("唔... 有点困惑呢");
    }
};

// 示例角色：小精灵
class Fairy : public KawaiiCharacter {
public:
    Fairy() : KawaiiCharacter("皮皮") {}
    
    std::string introduce() override {
        return speak("我是森林的小精灵，会发光的那种哦！");
    }
    
    std::string reactTo(const std::string& event) override {
        if (event == "森林") {
            return speak("听！是风的声音，还有鸟儿的歌唱！");
        } else if (event == "魔法") {
            return speak("让我为你施展一个小小的祝福魔法吧！");
        }
        return speak("（快乐地转圈圈）");
    }
};

int main() {
    std::vector<std::unique_ptr<KawaiiCharacter>> characters;
    characters.push_back(std::make_unique<MagicalGirl>());
    characters.push_back(std::make_unique<Fairy>());
    
    // 角色介绍
    for (auto& character : characters) {
        std::cout << character->introduce() << std::endl;
    }
    
    std::cout << "\n=== 场景：来到魔法森林 ===\n";
    for (auto& character : characters) {
        std::cout << character->reactTo("森林") << std::endl;
    }
    
    return 0;
}