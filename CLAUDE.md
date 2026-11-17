# ClickerGame: working rules for Claude

C++17 clicker game using SFML 3.1.0. A solo, learning-focused project, being turned into an incremental clicker with levels and upgrades.

## Git: the user is in control
- **Never commit, push or merge.** That includes `git push origin --delete`, or anything else that changes GitHub. Give the user the command to run instead.
- Creating local branches is fine and wanted.
- After each task, stage the changes and suggest a commit message. The user reviews and commits.
- Commit messages are a single message with no `Co-Authored-By` line. This overrides the default attribution.
- Don't remind the user to push after each task. The user pushes the branch only at the end of a session or when they intend to merge it. Mention `git push` only at those points.
- Use one long-running branch per big theme, with a micro commit per task. Don't open a PR per task or per session. Suggest a PR description only when the whole theme is done. Recommend "Create a merge commit", not squash.
- `docs/refactor-plan.md` and `docs/decisions.md` are tracked project records (our plan and decisions). Never commit them as their own commit. Stage them together with the code change of the step they belong to.

## How to work with me
- Work autonomously: make the routine decisions and get the code working. Don't ask me to approve every edit.
- **Any design decision we haven't already discussed:** ask me before deciding, not after. I want to understand it, and choose if I don't like your option. Ask in plain chat, not with the question-popup tool, so I can ask follow-ups.
- After each step, stop and give a **short** explanation focused on the decisions and why, plus the alternatives you didn't take. No line-by-line walkthroughs or long rename lists. Use clickable file:line links.
- Record every agreed decision about the game's design and code in `docs/decisions.md` (Chose / Instead of / Why). Not admin or workflow things like git, PRs or how we work together; those go in this file.
- I often forget how to run the game, so remind me when I need to test.

## Code quality
- Best practice but minimal. Don't over-engineer. No shortcuts from YouTube or beginner tutorials.
- Prefer one file per job, as long as that isn't over-engineering.
- Naming: PascalCase types; camelCase functions, parameters and locals; `m_` + camelCase members (`m_hpMax`); no `this->`. Tabs, short `//` comments.
- Add new `.cpp` files to `add_executable` in `CMakeLists.txt`.
- Standard library and SFML headers go in `src/stdafx.h` (the precompiled header), never as `#include <...>` in individual files.
- Build with `cmake --build build --config Debug`, and leave no new warnings.

## Current work
- The decoupling refactor (`docs/refactor-plan.md`) is finished and merged. Step 8 (events) was deferred, see `docs/decisions.md`.
- Keep using one subagent per task with you as the controller, to keep your context clean. Use cheaper models when a task doesn't need the best one. You run the builds and tests yourself, not the subagents. Run tasks one at a time, not in parallel.

## Later
- Levels and a larger upgrade system.
- Possibly move content to JSON once the game gets bigger.
