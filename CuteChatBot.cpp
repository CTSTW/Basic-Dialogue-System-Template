#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cctype>

// 颜色代码（Windows控制台）
#ifdef _WIN32
#include <windows.h>
class ConsoleColor {
private:
    HANDLE hConsole;
public:
    ConsoleColor() { hConsole = GetStdHandle(STD_OUTPUT_HANDLE); }
    void setColor(int color) { SetConsoleTextAttribute(hConsole, color); }
    void reset() { SetConsoleTextAttribute(hConsole, 7); } // 默认白色
};
#else
// Linux/Mac的颜色代码
class ConsoleColor {
public:
    void setColor(int color) {
        const char* colors[] = {
            "\033[0m",      // 重置
            "\033[31m",     // 红
            "\033[32m",     // 绿
            "\033[33m",     // 黄
            "\033[34m",     // 蓝
            "\033[35m",     // 紫
            "\033[36m",     // 青
            "\033[37m"      // 白
        };
        if (color >= 0 && color < 8) std::cout << colors[color];
    }
    void reset() { setColor(0); }
};
#endif

// 可爱的表情符号库
struct Emoji {
    static std::vector<std::string> happy;
    static std::vector<std::string> sad;
    static std::vector<std::string> love;
    static std::vector<std::string> surprise;
    
    static void initialize() {
        happy = {"(*^▽^*)", "(≧∇≦)ﾉ", "ヽ(✿ﾟ▽ﾟ)ノ", "(๑•̀ㅂ•́)و✧", "٩(◕‿◕｡)۶"};
        sad = {"(；ω；)", "(╥﹏╥)", "(ノдヽ)", "(；一_一)", "(-ω-；)"};
        love = {"(ﾉ´ з `)ノ", "(●´З｀●)", "(♡μ_μ)", "(*´∀｀*)", "(灬º‿º灬)♡"};
        surprise = {"Σ(°△°|||)︴", "(⊙ˍ⊙)", "（・□・；）", "(ﾟДﾟ;)", "ヽ(°〇°)ﾉ"};
    }
};

std::vector<std::string> Emoji::happy;
std::vector<std::string> Emoji::sad;
std::vector<std::string> Emoji::love;
std::vector<std::string> Emoji::surprise;

// 角色类
class KawaiiCharacter {
private:
    std::string name;
    std::string personality;
    int affection; // 好感度 0-100
    int energy;    // 精力 0-100
    std::map<std::string, std::vector<std::string>> dialogueMap;
    std::random_device rd;
    std::mt19937 gen;
    
    // 获取随机表情
    std::string getRandomEmoji(const std::vector<std::string>& emojiList) {
        std::uniform_int_distribution<> dis(0, emojiList.size() - 1);
        return emojiList[dis(gen)];
    }
    
    // 打字机效果显示文本
    void typewriterPrint(const std::string& text, int delay = 30) {
        for (char c : text) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        std::cout << std::endl;
    }
    
    // 彩色输出
    void coloredPrint(const std::string& text, int color, bool useTypewriter = true) {
        ConsoleColor console;
        console.setColor(color);
        
        if (useTypewriter) {
            typewriterPrint(text);
        } else {
            std::cout << text << std::endl;
        }
        
        console.reset();
    }
    
public:
    KawaiiCharacter(const std::string& n, const std::string& p) 
        : name(n), personality(p), affection(50), energy(80), gen(rd()) {
        
        Emoji::initialize();
        initializeDialogue();
    }
    
    void initializeDialogue() {
        // 问候语
        dialogueMap["greeting"] = {
            "你好呀，我是" + name + "！今天天气真好呢~",
            "喵~ 你来啦！我等你好久了" + getRandomEmoji(Emoji::happy),
            "呜哇！你突然出现吓我一跳" + getRandomEmoji(Emoji::surprise)
        };
        
        // 询问姓名
        dialogueMap["ask_name"] = {
            "可以告诉我你的名字吗？我会好好记住的！",
            "你叫什么名字呀？我想用特别的名字称呼你~",
            "告诉我你的名字嘛，这样我们就是朋友了！" + getRandomEmoji(Emoji::love)
        };
        
        // 日常对话
        dialogueMap["daily"] = {
            "今天有没有好好吃饭呀？要记得按时吃饭哦~",
            "我最近在看一本有趣的书，要一起看吗？",
            "你知道吗？星星在晚上会眨眼睛呢！",
            "好想和你一起去吃草莓蛋糕呀" + getRandomEmoji(Emoji::love),
            "我有点困了... Zzz..." + getRandomEmoji(Emoji::sad)
        };
        
        // 询问心情
        dialogueMap["ask_mood"] = {
            "你今天心情怎么样呀？要和我分享吗？",
            "有什么开心的事情吗？让我也高兴一下！",
            "如果有什么烦恼，可以跟我说哦~"
        };
        
        // 游戏相关
        dialogueMap["game"] = {
            "我们来玩游戏吧！猜猜我在想什么？",
            "石头剪刀布！我出...布！你输了！" + getRandomEmoji(Emoji::happy),
            "要听我唱歌吗？虽然可能有点跑调..."
        };
        
        // 告别
        dialogueMap["farewell"] = {
            "这么快就要走了吗？我会想你的...",
            "再见啦！明天也要来找我玩哦~",
            "要照顾好自己，我们下次见！" + getRandomEmoji(Emoji::love)
        };
        
        // 随机回应
        dialogueMap["random"] = {
            "诶？这个问题好难回答...让我想想...",
            "不知道呢，不过和你聊天很开心！",
            "今天也是个美好的日子呢~",
            "你看，窗外有只小鸟飞过去了！",
            "我好喜欢你和我聊天" + getRandomEmoji(Emoji::love)
        };
    }
    
    void speak(const std::string& category) {
        std::string emoji = "";
        
        // 根据好感度调整语气
        if (affection > 70) {
            emoji = " " + getRandomEmoji(Emoji::love);
        } else if (affection > 40) {
            emoji = " " + getRandomEmoji(Emoji::happy);
        } else {
            emoji = " " + getRandomEmoji(Emoji::sad);
        }
        
        // 获取对话
        std::string speech;
        if (dialogueMap.find(category) != dialogueMap.end() && !dialogueMap[category].empty()) {
            std::uniform_int_distribution<> dis(0, dialogueMap[category].size() - 1);
            speech = dialogueMap[category][dis(gen)];
        } else {
            std::uniform_int_distribution<> dis(0, dialogueMap["random"].size() - 1);
            speech = dialogueMap["random"][dis(gen)];
        }
        
        // 显示角色名和对话
        ConsoleColor console;
        console.setColor(13); // 紫色
        
        std::cout << "\n┌─【" << name << "】";
        for (int i = name.length(); i < 15; i++) std::cout << "─";
        std::cout << "┐" << std::endl;
        
        console.setColor(11); // 青色
        std::cout << "│ ";
        typewriterPrint(speech + emoji);
        
        console.setColor(13);
        std::cout << "└";
        for (int i = 0; i < 20; i++) std::cout << "─";
        std::cout << "┘" << std::endl;
        
        console.reset();
        
        // 更新状态
        energy -= 5;
        if (energy < 0) energy = 0;
    }
    
    void respondToInput(const std::string& input) {
        std::string lowercase = input;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(),
                      [](unsigned char c){ return std::tolower(c); });
        
        // 检查关键词并回应
        if (lowercase.find("你好") != std::string::npos || 
            lowercase.find("嗨") != std::string::npos ||
            lowercase.find("hello") != std::string::npos) {
            affection += 5;
            speak("greeting");
        }
        else if (lowercase.find("名字") != std::string::npos ||
                 lowercase.find("叫") != std::string::npos) {
            affection += 3;
            speak("ask_name");
        }
        else if (lowercase.find("心情") != std::string::npos ||
                 lowercase.find("感觉") != std::string::npos) {
            speak("ask_mood");
        }
        else if (lowercase.find("游戏") != std::string::npos ||
                 lowercase.find("玩") != std::string::npos) {
            energy += 10;
            if (energy > 100) energy = 100;
            speak("game");
        }
        else if (lowercase.find("再见") != std::string::npos ||
                 lowercase.find("拜拜") != std::string::npos ||
                 lowercase.find("bye") != std::string::npos) {
            speak("farewell");
            return;
        }
        else if (lowercase.find("喜欢") != std::string::npos ||
                 lowercase.find("爱") != std::string::npos) {
            affection += 10;
            speak("random");
        }
        else {
            affection += 1;
            speak("daily");
        }
        
        // 显示状态
        showStatus();
    }
    
    void showStatus() {
        ConsoleColor console;
        console.setColor(10); // 绿色
        
        std::cout << "\n【状态】";
        std::cout << " 好感度: ";
        
        // 好感度条
        console.setColor(12); // 红色
        int bars = affection / 5;
        for (int i = 0; i < 20; i++) {
            if (i < bars) {
                std::cout << "♥";
            } else {
                std::cout << "♡";
            }
        }
        
        console.setColor(10);
        std::cout << " " << affection << "/100";
        
        console.setColor(14); // 黄色
        std::cout << "  精力: ";
        
        // 精力条
        console.setColor(11); // 青色
        bars = energy / 5;
        for (int i = 0; i < 20; i++) {
            if (i < bars) {
                std::cout << "★";
            } else {
                std::cout << "☆";
            }
        }
        
        console.setColor(14);
        std::cout << " " << energy << "/100" << std::endl;
        
        console.reset();
    }
    
    void showIntroduction() {
        ConsoleColor console;
        console.setColor(13); // 紫色
        
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════╗" << std::endl;
        console.setColor(11);
        std::cout << "║         ✨ 可爱的聊天机器人 ✨              ║" << std::endl;
        console.setColor(13);
        std::cout << "╠════════════════════════════════════════════╣" << std::endl;
        console.setColor(10);
        std::cout << "║ 角色: " << name;
        for (int i = name.length(); i < 38; i++) std::cout << " ";
        std::cout << "║" << std::endl;
        console.setColor(14);
        std::cout << "║ 性格: " << personality;
        for (int i = personality.length(); i < 38; i++) std::cout << " ";
        std::cout << "║" << std::endl;
        console.setColor(13);
        std::cout << "╚════════════════════════════════════════════╝" << std::endl;
        
        console.reset();
        
        // 显示帮助
        console.setColor(8); // 灰色
        std::cout << "\n【你可以对我说】" << std::endl;
        std::cout << "• 你好 / 嗨 - 打招呼" << std::endl;
        std::cout << "• 关于名字 - 询问我的名字" << std::endl;
        std::cout << "• 心情相关 - 分享心情" << std::endl;
        std::cout << "• 玩游戏 - 一起玩耍" << std::endl;
        std::cout << "• 喜欢/爱 - 表达感情" << std::endl;
        std::cout << "• 再见 - 结束对话" << std::endl;
        std::cout << "• (其他任何话) - 自由聊天" << std::endl;
        
        console.reset();
    }
    
    // 喂食恢复精力
    void feed(const std::string& food) {
        ConsoleColor console;
        console.setColor(6); // 橙色
        
        if (food == "蛋糕" || food == "草莓蛋糕") {
            energy += 30;
            affection += 15;
            std::cout << "\n🍰 " << name << ": \"哇！是最喜欢的草莓蛋糕！太开心了！\" ";
            std::cout << getRandomEmoji(Emoji::love) << std::endl;
        }
        else if (food == "饼干" || food == "曲奇") {
            energy += 20;
            affection += 10;
            std::cout << "\n🍪 " << name << ": \"饼干好香呀！谢谢你！\" ";
            std::cout << getRandomEmoji(Emoji::happy) << std::endl;
        }
        else if (food == "咖啡" || food == "茶") {
            energy += 15;
            affection += 5;
            std::cout << "\n☕ " << name << ": \"暖暖的饮料，感觉精神多了！\" ";
            std::cout << getRandomEmoji(Emoji::happy) << std::endl;
        }
        else {
            energy += 10;
            affection += 3;
            std::cout << "\n🍴 " << name << ": \"" << food << "吗？谢谢你！\" ";
            std::cout << getRandomEmoji(Emoji::happy) << std::endl;
        }
        
        if (energy > 100) energy = 100;
        if (affection > 100) affection = 100;
        
        showStatus();
        console.reset();
    }
    
    int getEnergy() const { return energy; }
    int getAffection() const { return affection; }
};

// 游戏主循环
int main() {
    // 设置控制台编码（Windows）
#ifdef _WIN32
    SetConsoleOutputCP(65001); // UTF-8
#endif
    
    ConsoleColor console;
    
    // 显示欢迎界面
    console.setColor(13);
    std::cout << "\n";
    std::cout << "███████╗██╗   ██╗████████╗███████╗    ██████╗ ██╗      ██████╗ ████████╗" << std::endl;
    std::cout << "██╔════╝██║   ██║╚══██╔══╝██╔════╝    ██╔══██╗██║     ██╔═══██╗╚══██╔══╝" << std::endl;
    std::cout << "█████╗  ██║   ██║   ██║   █████╗      ██████╔╝██║     ██║   ██║   ██║   " << std::endl;
    std::cout << "██╔══╝  ██║   ██║   ██║   ██╔══╝      ██╔═══╝ ██║     ██║   ██║   ██║   " << std::endl;
    std::cout << "██║     ╚██████╔╝   ██║   ███████╗    ██║     ███████╗╚██████╔╝   ██║   " << std::endl;
    std::cout << "╚═╝      ╚═════╝    ╚═╝   ╚══════╝    ╚═╝     ╚══════╝ ╚═════╝    ╚═╝   " << std::endl;
    
    console.setColor(11);
    std::cout << "\n                     欢迎来到可爱聊天室！\n" << std::endl;
    console.reset();
    
    // 创建角色
    KawaiiCharacter character("小喵", "傲娇又粘人的猫咪女孩");
    
    // 显示角色介绍
    character.showIntroduction();
    
    // 主对话循环
    std::string input;
    bool running = true;
    
    while (running) {
        console.setColor(15); // 白色
        std::cout << "\n【你】> ";
        
        // 获取用户输入
        std::getline(std::cin, input);
        
        if (input.empty()) {
            std::cout << "（请不要输入空内容哦~）" << std::endl;
            continue;
        }
        
        // 特殊命令
        if (input == "/help") {
            console.setColor(8);
            std::cout << "\n【特殊命令】" << std::endl;
            std::cout << "/help     - 显示帮助" << std::endl;
            std::cout << "/feed     - 喂食" << std::endl;
            std::cout << "/status   - 查看状态" << std::endl;
            std::cout << "/quit     - 退出" << std::endl;
            console.reset();
            continue;
        }
        else if (input == "/feed") {
            console.setColor(6);
            std::cout << "\n喂什么呢？(蛋糕/饼干/咖啡/茶/其他): ";
            std::string food;
            std::getline(std::cin, food);
            character.feed(food);
            continue;
        }
        else if (input == "/status") {
            character.showStatus();
            continue;
        }
        else if (input == "/quit") {
            console.setColor(12);
            std::cout << "\n真的要离开吗？(y/n): ";
            std::string confirm;
            std::getline(std::cin, confirm);
            
            if (confirm == "y" || confirm == "Y" || confirm == "是") {
                character.respondToInput("再见");
                running = false;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            continue;
        }
        
        // 检查精力
        if (character.getEnergy() <= 0) {
            console.setColor(12);
            std::cout << "\n😴 " << character.getEnergy() << " 精力用尽了！需要喂食恢复精力！" << std::endl;
            std::cout << "使用 /feed 命令来喂食" << std::endl;
            console.reset();
            continue;
        }
        
        // 正常对话
        character.respondToInput(input);
        
        // 检查是否应该结束
        if (input.find("再见") != std::string::npos ||
            input.find("拜拜") != std::string::npos ||
            input.find("bye") != std::string::npos) {
            
            console.setColor(12);
            std::cout << "\n对话结束，按回车键退出..." << std::endl;
            std::cin.get();
            running = false;
        }
    }
    
    // 结束画面
    console.setColor(13);
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════╗" << std::endl;
    console.setColor(11);
    std::cout << "║        感谢使用可爱聊天机器人！            ║" << std::endl;
    console.setColor(10);
    std::cout << "║        期待与你的下一次相遇~               ║" << std::endl;
    console.setColor(13);
    std::cout << "╚════════════════════════════════════════════╝" << std::endl;
    console.reset();
    
    return 0;
}
