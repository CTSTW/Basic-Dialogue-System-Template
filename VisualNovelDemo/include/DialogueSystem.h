#pragma once
#ifndef DIALOGUE_SYSTEM_H
#define DIALOGUE_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace VisualNovel {
    
    // 对话行结构
    struct DialogueLine {
        std::string characterName;
        std::string characterDisplayName;
        std::string text;
        std::string voiceFile;
        std::string expression;
        std::string position;  // left, center, right
        int fontSize;
        std::string textColor;
        float displaySpeed;
        std::vector<std::string> effects;  // 特效列表
        std::map<std::string, std::string> metadata;
        
        DialogueLine();
    };
    
    // 选择支结构
    struct ChoiceOption {
        std::string text;
        std::string targetLabel;
        std::vector<std::string> conditions;  // 显示条件
        std::vector<std::string> effects;     // 选择后的效果
        
        ChoiceOption(const std::string& t, const std::string& target);
    };
    
    // 对话历史记录
    struct DialogueHistory {
        std::vector<DialogueLine> lines;
        int maxHistorySize;
        
        DialogueHistory(int maxSize = 100);
        void AddLine(const DialogueLine& line);
        void Clear();
        const std::vector<DialogueLine>& GetHistory() const;
    };
    
    // 对话系统核心类
    class DialogueSystem {
    private:
        std::vector<DialogueLine> currentDialogue;
        std::vector<ChoiceOption> currentChoices;
        DialogueLine currentLine;
        DialogueHistory history;
        
        int currentLineIndex;
        bool isTyping;
        float typingProgress;
        float typingSpeed;
        bool autoPlay;
        float autoPlayTimer;
        
        // 文本效果处理器
        class TextEffectProcessor {
        private:
            std::map<std::string, std::function<void(DialogueLine&, float)>> effects;
            
        public:
            TextEffectProcessor();
            void RegisterEffect(const std::string& name, 
                              std::function<void(DialogueLine&, float)> effect);
            void ApplyEffects(DialogueLine& line, float deltaTime);
        };
        
        TextEffectProcessor effectProcessor;
        
    public:
        DialogueSystem();
        ~DialogueSystem();
        
        // 对话控制
        void StartDialogue(const std::vector<DialogueLine>& dialogue);
        void NextLine();
        void PreviousLine();
        void JumpToLine(int index);
        void SkipToEnd();
        
        // 文本显示
        void Update(float deltaTime);
        bool IsTypingComplete() const;
        void CompleteTyping();
        const DialogueLine& GetCurrentLine() const;
        
        // 选择支
        void SetChoices(const std::vector<ChoiceOption>& choices);
        const std::vector<ChoiceOption>& GetChoices() const;
        void ClearChoices();
        bool HasChoices() const;
        
        // 自动播放
        void SetAutoPlay(bool enabled);
        bool IsAutoPlaying() const;
        void SetAutoPlayDelay(float delay);
        
        // 历史记录
        const DialogueHistory& GetHistory() const;
        void ClearHistory();
        
        // 设置
        void SetTypingSpeed(float speed);
        float GetTypingSpeed() const;
        
        // 特效
        void RegisterTextEffect(const std::string& name,
                              std::function<void(DialogueLine&, float)> effect);
        
    private:
        void ProcessTextEffects(float deltaTime);
        void FormatTextWithEffects();
    };
    
} // namespace VisualNovel

#endif // DIALOGUE_SYSTEM_H
