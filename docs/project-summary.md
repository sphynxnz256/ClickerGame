# ClickerGame: project summary for a new agent

Read this first, then `CLAUDE.md` (the working rules, loaded automatically in Claude Code), then skim `docs/decisions.md`. This file is a snapshot as of 2026-11-27. If it disagrees with the code or git history, trust those and tell the user.

## What this project is
A C++17 clicker game using SFML 3.1.0, built on Windows with CMake and VS Code. The player clicks an enemy to damage it, collects coins that drop, and spends them on damage upgrades. Every tenth kill spawns a boss with a time limit. It is a solo, learning-focused project that is being turned into an incremental game with levels and a larger upgrade system.

## Who you are working with
- The user is a solo developer. They want to understand the architecture and the reasons for each choice well enough to work in the industry and explain their own code in a code review. The goal is understanding, not only a working game.
- They plan to review the code of each change after it is made (the files that change touched, line by line), so that they could write it themselves. They do not want a one-off full code review.
- They aim to finish at least one task a day, so prefer small tasks that fit in a session.
- They commit and push themselves. You never run `git commit`, `git push` or `git merge`.

## How to work with the user (the full list is in CLAUDE.md)
- **Plan first, then wait for an explicit "go".** Show the plan: the files that will be touched and the data flow right back to `main.cpp`, plus the decisions that need making. Do not edit code, create branches or start subagents until the user says go. "OK, let's do this" while discussing is not a go.
- **Ask about any design decision not already discussed, in plain chat.** Give the options with explicit upsides and downsides and a recommendation. Do not use the question-popup tool. The user makes the final call and often asks follow-up questions.
- **Follow the conventions already in the code.** Do not introduce new conventions or language features (namespaces, `static constexpr`, and so on) without asking. Check git history before calling something existing precedent (the refactor introduced several patterns; see the end of this file).
- **After each task, stop with a short explanation:** the decisions made, why, and the alternatives not taken. Use clickable `file:line` links. When the user asks for a line-by-line walkthrough, give one, and end with a "check yourself" question.
- **Use a subagent per task, with you as the controller** (cheaper models where the task allows). You run the builds and tests yourself, not the subagents. One task at a time.
- **Staging and git:** after a task, stage the changes and suggest a one-line commit message. No `Co-Authored-By` line. Do not remind the user to push after each task. Mention `git push` only at the end of a session or when they intend to merge.
- **Docs:** `docs/decisions.md` records agreed game and code decisions only (Chose / Instead of / Why / My thinking), not git, PR or workflow rules. Never commit the `docs/` files on their own. Stage them with the code change they belong to.
- **PRs:** one long-running branch per theme, one micro commit per task, one PR when the whole theme is done. Recommend "Create a merge commit", not squash. PR descriptions are short: a Summary and a What changed list, with no future plans, notes or attribution lines.
- **README.md is the user's own words. Never edit it.** AI-written docs go elsewhere (`BUILDING.md`, `docs/`).
- **Remind the user how to run the game when they need to test** (see Build and run).

## Build and run
```
cmake --build build --config Debug
cd build\Debug
.\ClickerGame.exe
```
- Run it from `build\Debug`. The build copies `assets/` (fonts and textures) next to the exe, and the game loads them by relative path. From the project root it exits with an error, by design.
- Leave no new warnings. Add new `.cpp` files to `add_executable` in `CMakeLists.txt`.
- A missing font stops the game with an error and exit code 1. This is deliberate (fail fast).

## Code conventions
- PascalCase types; camelCase functions, parameters and locals; `m_` + camelCase members (`m_hpMax`); no `this->`. Tabs, short `//` comments.
- Per-object values (including tuning numbers) are plain private members set in an `initVariables()` function. Larger setup is split into `initX()` functions called from the constructor.
- All standard library and SFML headers go in `src/stdafx.h` (the precompiled header), never as `#include <...>` in individual files.
- Best practice but minimal. Do not over-engineer. One file per job where that is not over-engineering.

## Architecture
Responsibilities are split by folder. Rules and state know nothing about SFML drawing.

| Folder | Role |
|---|---|
| `src/state/` | Plain data, the single source of truth: `PlayerState` (damage, coins, upgradeCost), `EnemyState` (a pointer to an `EnemyType` plus current hp), `GameState` (player, enemy, `Phase`, killCount, bossTimeLeft). |
| `src/content/` | Game data in C++: `EnemyType` (the definition struct) and `EnemyData.h` (`content::regularEnemy`, `content::boss`). |
| `src/rules/` | Free functions in `namespace rules`, each taking only the state it needs: `Combat` (`clickDamage`, `hitEnemy`, `rollCoinDrop`, `collectCoin`), `Upgrades` (`tryBuyDamageUpgrade`), `BossTimer` (`updateBossTimer`, `retreat`), `Spawning` (`startingState`, `spawnEnemy`, `finishDeath`, `finishEscape`). |
| `src/view/` | Drawing: `TextureCache`, `EnemyView`, `HudView` (coins, damage, upgrade cost, buttons), `HpBarView`, `BossCountdownView`, `Button`, `DeathAnimation`, `SmokeAnimation`, `NumberFormat` (`formatNumber`). Views read `GameState` every frame. |
| `src/entities/` | `Coin`, which owns its own movement and state (it predates the refactor). |
| `src/core/` | `Game` (window, loop, input routing, wiring), `RNG` (shared random generator), `Level` (an empty stub; see below). |

### Data flow
1. `main.cpp` makes one `RNG` and one `Game`, then loops `game.update()` and `game.render()` inside a `try`/`catch` that prints startup errors.
2. `Game` owns everything as plain members built in its constructor's init list, in declaration order: `m_rng` (a reference), `m_window`, `m_frameClock`, `m_textures`, `m_state`, `m_font`, `m_enemyView`, `m_hud`, `m_hpBar`, `m_bossCountdown`, the two animations, `m_background`, `m_coinsVector`. There is no raw `new` or `delete`, and no destructor.
3. `Game::update()`: measure `dt` once, poll the mouse position once, `polledEvents()` (a left click goes to `handleClick`), `updateCombat`, `updateAnimator`, `updateCoins`.
4. `handleClick` uses first-match-wins: the Upgrade button, then the Retreat button (boss fight only), then the enemy.
5. Enemy dies: `rules::hitEnemy` returns true, `Game::onEnemyKilled()` starts `DeathAnimation` and drops coins. The `Phase` goes Fighting, Dying, then `rules::finishDeath` spawns the next enemy (the boss every tenth kill) and returns to Fighting.
6. Boss timer expires or Retreat is clicked: `Phase::Escaping`, `Game::onBossEscaped()` starts `SmokeAnimation`, then `rules::finishEscape` spawns a regular enemy.
7. `updateCoins(dt)` moves each coin, then collects any coin that is collectable and under the cursor (only while the window has focus), using erase-remove. `rules::collectCoin` adds to `player.coins`.
8. `Game::render()` draws the background, enemy, animations and coins, then the views (`renderUI`), which read `m_state`.

## Gameplay numbers (current)
- Window 800x600. The enemy is centred. Player starts with damage 1, coins 0, upgrade cost 10. An upgrade doubles damage and doubles the cost.
- Regular enemy: HP 10, drops 1-2 coins, scale 0.25, five textures picked at random each spawn (slime, crab, rat, raven, wasp). Boss: HP 250, drops 10-20 coins, scale 0.5, jellyfish texture, 15 second time limit. Boss after every 10 kills (`EnemyType::killsToSpawn` on `content::boss`).
- Coin value is hard-coded as 1.0 in `Coin::initVariables`; nothing scales it yet.
- Numbers on screen go through `formatNumber` (whole numbers for now; K/M/B suffixes are possible later).

### Coin behaviour
- **Pickup:** by mouse-over, polled once per frame. A coin is collectable after `m_pickupDelay` (0.3 s), so it can be caught in flight but not instantly at the spawn point (the mouse sits on the enemy's centre). Clicking no longer collects coins, so a click always reaches the enemy.
- **Flight:** projectile motion. Each coin gets a random launch speed (450-650 px/s) and angle (45-135 degrees, 90 is straight up) and gravity 2200 px/s^2. Position is a formula of elapsed time, `x = x0 + vx*t`, `y = y0 + vy*t + 0.5*g*t^2`, so it never curves back. It stops on a floor line 150 px above the window bottom. Flights last about 0.57-0.79 s.
- **The ranges were checked** (every whole-number speed and angle) so coins never leave the screen, assuming an 800x600 window, the enemy centred and a 56 px coin. The farthest landing is 304 px from the centre against 352 px of room. Recheck them if the window size, the enemy position or the coin size changes. The values are in `Coin::initVariables`.

## History
- Early work: the project moved from Visual Studio to VS Code and CMake, and was renamed from BasicRPG to ClickerGame. SFML was upgraded from 2.6.2 to 3.1.0 (PR #6).
- **Decoupling refactor (done, merged as PR #7):** 11 steps, see `docs/refactor-plan.md` (kept as a record). Naming cleanup, one frame clock, clicks from events, a texture cache, one enemy type, `GameState` and `Phase`, `double` numbers, rules into `rules/`, views that read state, `Animator` split into two classes, and `Game` tidied. Step 8 (events) was deferred.
- **`coin-updates` (done, merged):**
  1. Collect coins by mouse-over after a short pickup delay.
  2. Use private members for constants instead of namespaces and `static constexpr`.
  3. Throw coins as projectiles with a random launch angle and speed.
- **Current branch `title-screen`:** a title screen with New Game, Load Game, Save Game and Exit buttons. Started with two small cleanups (unused `Coin` members removed, `killsToSpawn` moved onto the boss type).

## Decisions (details and reasoning in docs/decisions.md)
SFML 3.1 first; naming style; state / content / rules / view / core architecture; enemies as data (Type Object pattern); content in C++, not JSON; levels left out of the refactor; one frame clock; clicks from events; texture cache owned by `Game`; one current enemy with a boss flag on the type; `double` for big numbers; rules as free functions on the smallest state; events deferred; views read state every frame and the UI split by job; `Animator` split (the death animation copies the enemy sprite, the smoke takes a centre point in `start`); `Game` owns its parts as plain members and a missing font fails fast; the first enemy comes from `rules::startingState()`; coins collected by mouse-over with a delay; coins fly as projectiles. Read the file for the alternatives and the user's own reasoning.

## Known leftovers and quirks
- `src/core/Level.h/.cpp` is an empty stub, listed in `CMakeLists.txt`, ready for the levels work.
- `namespace rules`, `namespace content`, `inline const EnemyType` in a header, `enum class Phase`, `std::optional` members, and the `remove_if` lambda in `updateCoins` were introduced during or after the refactor. The user accepted them in discussion and wants them kept. `static constexpr` members and anonymous namespaces were introduced without discussion and have been removed. Ask before adding any new convention.
- There are no automated tests. Verification is a clean build, then the user plays the game.

## Ideas not started (the "Later" list in CLAUDE.md)
- Levels and a larger upgrade system. Coin value should grow, not mostly coin count. The user has said they never want thousands of entities on screen.
- Possibly move content to JSON once the game gets bigger.
- Pickup ideas for upgrades: auto-collect after a few seconds, a pickup radius, or a magnet.
- Events (a `Signal` listener list) once a second independent system needs to react to something, such as sound, stats, achievements or level progress.
