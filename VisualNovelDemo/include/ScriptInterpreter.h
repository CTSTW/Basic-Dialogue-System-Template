#pragma once
#ifndef SCRIPT_INTERPRETER_H
#define SCRIPT_INTERPRETER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "DialogueSystem.h"

namespace VisualNovel {
    
    // 脚本命令类型
    enum class CommandType {
        DIALOGUE,
        JUMP,
        LABEL,
        CONDITION,
        CHOICE,
        SET_VARIABLE,
        SET_FLAG,
        PLAY_SOUND,
        PLAY_BGM,
        STOP_BGM,
        SHOW_CHARACTER,
        HIDE_CHARACTER,
        CHANGE_BACKGROUND,
        WAIT,
        ANIMATION,
        SPECIAL_EFFECT,
        CALL,
        RETURN,
        END
    };
    
    // 脚本命令
    struct ScriptCommand {
        CommandType type;
        std::vector<std::string> parameters;
        std::string rawLine;
        int lineNumber;
        
        ScriptCommand(CommandType t, const std::vector<std::string>& params, 
                     const std::string& raw, int line);
    };
    
    // 脚本标签
    struct ScriptLabel {
        std::string name;
        int position;
    };
    
    // 脚本变量作用域
    class VariableScope {
    private:
        std::map<std::string, std::string> variables;
        std::map<std::string, bool> flags;
        VariableScope* parent;
        
    public:
        VariableScope(VariableScope* parent = nullptr);
        
        void SetVariable(const std::string& name, const std::string& value);
        std::string GetVariable(const std::string& name) const;
        bool HasVariable(const std::string& name) const;
        
        void SetFlag(const std::string& name, bool value);
        bool GetFlag(const std::string& name) const;
        bool HasFlag(const std::string& name) const;
        
        void Clear();
    };
    
    // 脚本解释器
    class ScriptInterpreter {
    private:
        std::vector<ScriptCommand> commands;
        std::map<std::string, ScriptLabel> labels;
        std::map<std::string, std::function<bool(const std::vector<std::string>&)>> customCommands;
        
        VariableScope* currentScope;
        std::vector<VariableScope*> scopeStack;
        
        int programCounter;
        bool isRunning;
        bool isWaiting;
        float waitTimer;
        
        // 调用栈
        struct CallStackFrame {
            int returnAddress;
            VariableScope* scope;
        };
        std::vector<CallStackFrame> callStack;
        
    public:
        ScriptInterpreter();
        ~ScriptInterpreter();
        
        // 脚本加载
        bool LoadScript(const std::string& scriptPath);
        bool LoadScriptFromString(const std::string& scriptContent);
        void ClearScript();
        
        // 执行控制
        void Start();
        void Stop();
        void Pause();
        void Resume();
        void Update(float deltaTime);
        
        // 跳转控制
        void JumpToLabel(const std::string& label);
        void JumpToLine(int lineNumber);
        void CallLabel(const std::string& label);
        void Return();
        
        // 变量操作
        void SetVariable(const std::string& name, const std::string& value);
        std::string GetVariable(const std::string& name) const;
        void SetFlag(const std::string& name, bool value);
        bool GetFlag(const std::string& name) const;
        
        // 自定义命令
        void RegisterCustomCommand(const std::string& commandName,
                                 std::function<bool(const std::vector<std::string>&)> handler);
        
        // 状态查询
        bool IsRunning() const;
        bool IsWaiting() const;
        int GetCurrentLine() const;
        const std::string& GetCurrentCommand() const;
        
        // 获取解析结果
        const std::vector<DialogueLine>& GetDialogueLines() const;
        const std::vector<ChoiceOption>& GetChoiceOptions() const;
        
    private:
        bool ParseLine(const std::string& line, int lineNumber);
        ScriptCommand CreateCommand(const std::string& line, int lineNumber);
        bool ExecuteCommand(const ScriptCommand& command);
        
        // 命令处理器
        bool HandleDialogue(const std::vector<std::string>& params);
        bool HandleJump(const std::vector<std::string>& params);
        bool HandleCondition(const std::vector<std::string>& params);
        bool HandleChoice(const std::vector<std::string>& params);
        bool HandleSetVariable(const std::vector<std::string>& params);
        bool HandleSetFlag(const std::vector<std::string>& params);
        bool HandleWait(const std::vector<std::string>& params);
        
        // 工具函数
        std::string EvaluateExpression(const std::string& expr) const;
        bool EvaluateCondition(const std::string& condition) const;
        void PushScope();
        void PopScope();
        
        // 解析状态
        std::vector<DialogueLine> parsedDialogue;
        std::vector<ChoiceOption> parsedChoices;
    };
    
} // namespace VisualNovel

#endif // SCRIPT_INTERPRETER_H
