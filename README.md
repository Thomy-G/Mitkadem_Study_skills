# Bar-Ilan University — Advanced Systems Programming (893210)
## Interactive Exam Practice Simulator for Antigravity

This repository contains a custom agentic **Skill** designed for the **Antigravity IDE** to run interactive, realistic exam practice simulations. The system is custom-tailored to mimic the style, grading severity, and curriculum guidelines of the final exam papers (2025 Moed Alef/Bet and 2026 Moed Alef).

---

## 🌟 Key Features
- **4 Core Question Archetypes**:
  1. **C++ Architectural Design (40 Points)**: SOLID design principles, CLI stream processing, Command/Proxy patterns, and smart pointers.
  2. **Full-Stack MVC Debugging (20 Points)**: Finding syntax and architectural bugs in Node.js, Express routes/controllers, and Mongoose schemas.
  3. **Code Execution Tracing (20 Points)**: Multiple/Virtual inheritance layout (Diamond problem), JS event loop, async scheduling, hoisting, POSIX `fork()` trees, and thread sync.
  4. **Infrastructure & DevOps (20 Points)**: Docker layer caching optimizations and low-level Git object storage models (`.git/objects`).
- **Past Exam Vault**: Pre-loaded with official reconstructions from:
  - 2026 Semester A, Moed Alef
  - 2025 Semester B, Moed Alef
  - 2025 Semester B, Moed Bet
- **Structured Practice Sessions**: Automatically generates clean directory files for questions, user templates, and detailed architectural solutions.

---

## ⚙️ How to Install in Antigravity

Since Antigravity automatically detects workspace skills, installing this is as simple as placing the `.agents` folder in your workspace root:

### Step 1: Clone or Copy the Repository
Clone this repository directly, or copy the `.agents` folder into the root of your active project workspace:
```bash
git clone https://github.com/Thomy-G/Mitkadem_Study_skills.git
```
Your workspace folder should look like this:
```
Your-Workspace/
├── .agents/
│   └── skills/
│       └── biu-systems-exam-practice/
│           ├── SKILL.md
│           └── references/
│               ├── Exam_2025_Moed_Alef_Full_Spec.md
│               └── Exam_2026_Moed_A_Sample.md
├── Past_Exams/
└── README.md
```

### Step 2: Trigger the Exam Practice
Open the workspace in your Antigravity IDE. Once loaded, open a chat session with your AI Coding Assistant and say:
> *"Let's start the exam simulation"* or *"Test me using the biu-systems-exam-practice skill."*

---

## 📝 Practice Session Workflow
When you start a session, the simulator will:
1. Initialize a subfolder under `Practice_Sessions/` for the current topic (e.g. `Practice_Sessions/Telemetry_CLI/`).
2. Write a question file (`Telemetry_CLI_Question.md`) and an answer template (`Telemetry_CLI_Answer.cpp`).
3. You can write your code and description in the answer file.
4. When you are ready, save the file and say **"Review this"**.
5. The agent will grade your submission strictly (like a university grader) and write a comprehensive analysis and code solution into a solution file (`Telemetry_CLI_Solution.md`).
