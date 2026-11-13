# Decoupling refactor plan

Restructures the game so it's easy to extend with an upgrade system and levels. The game should look and play exactly the same once every step is done. No new features.

## Principles

- **Single source of truth:** each piece of game state, such as HP, coins or damage, lives in exactly one place. Everything else reads it from there, so nothing has to be kept in sync by hand.
- **Content is data:** enemy definitions and tuning numbers live in `content/`, not inside classes. A new enemy, upgrade or level should be a new data entry, not a new class.
- **Rules don't know about SFML:** game rules (combat, upgrades, boss timer, spawning) are plain C++ in `rules/`. Drawing and input stay in `view/` and `core/`.
- **Events only for "something happened":** use events where independent parts react to the same thing, such as an enemy dying or the boss escaping. Use direct function calls everywhere else.
- **Minimal:** build only what this version of the game uses, but leave room for upgrades and levels.

## Target structure

| Folder | Contents | Uses SFML |
|---|---|---|
| `src/state/` | `PlayerState`, `Enemy` (points to an `EnemyType`, holds current HP), `GameState` (player, current enemy, `Phase`, kill count, boss time left) | No |
| `src/content/` | Two `EnemyType`s and tuning constants (see below) | No |
| `src/rules/` | `Combat`, `clickDamage()`, `Upgrades`, `BossTimer`, `Spawning`, a small `Signal` listener list | No |
| `src/view/` | `TextureCache`, enemy drawing, HP bar, HUD, `Button`, coins, death animation, smoke animation | Yes |
| `src/core/` | `Game` (window, loop, input routing, wiring), `RNG` | Yes |

### Content

- **Regular enemy:** one `EnemyType`, with HP 10, drops 1-2 coins and scale 0.25. It has **five textures** (slime, crab, rat, raven, wasp), and one is picked at random each spawn, purely for variety. These aren't separate enemy types.
- **Boss:** one `EnemyType`, with HP 250, drops 10-20 coins, scale 0.5, the jellyfish texture and a 15-second time limit.
- **Tuning:** the boss appears after 10 kills. The player starts with 1 damage, 0 coins and an upgrade cost of 10. Each upgrade doubles damage and doubles the cost.

## Steps

Each step is one commit. After each step, the game must build and play exactly as before.

0. **Naming.** Rename all snake_case function parameters and local variables to camelCase, for example `target_size` → `targetSize`. Rename every member variable to use the `m_` prefix (`damage` → `m_damage`) and drop `this->` from member access. Rename only, with no other changes.
1. **One frame clock.** `Game` keeps a single `sf::Clock` and computes `dt` once per frame. `dt` is passed to everything that animates or counts down: the death and smoke animations, coin movement and the boss countdown. Remove the per-class clocks and the `firstFrame` workarounds.
2. **Clicks from events.** Handle `sf::Event::MouseButtonPressed` (left button) in the event loop, so each click is handled exactly once. Pass the click position to whatever needs it. Remove `mouseHeld`, `static click_processed`, `Button::isClicked`'s use of `sf::Mouse::isButtonPressed`, and the hold-toggling in `upgradeDamage()` and `retreat()`. Hover effects can keep reading the current mouse position.
3. **Texture cache.** Add `view/TextureCache`, which loads each texture file once and hands out references. `Enemy` and `Boss` use it, so `Enemy` no longer copies a whole `sf::Texture` on every reset.
4. **One enemy type.** Replace `Enemy` and `Boss` with `EnemyType` (definition data in `content/`) and `Enemy` (a pointer to its type plus current HP). Update `Game` to use the new types.
5. **State and `Phase`.** Add `PlayerState` (replacing `Player`) and `GameState`. Replace `bossActive`, `playDeathAnimation` and `playEscapeAnimation` with `enum class Phase { Fighting, Dying, Escaping }` plus a "boss fight" flag or enemy type check. Remove `Player`'s unused `attackCD` and `attackCDMax`.
6. **Big numbers.** Coins, damage, HP and upgrade cost become `double`. Text on screen must look the same (no decimals).
7. **Rules out of `Game`.** Move the game logic into `rules/`, using only `GameState` and `content/`, with no SFML:
   - `clickDamage(const PlayerState&)` returns the player's base damage. Upgrades will plug in here later.
   - `Combat`, `Upgrades`, `BossTimer` and `Spawning`. Spawning picks the next enemy and starts the boss after 10 kills.
8. **Events.** Add a small `Signal` listener list. Rules announce `enemyDied` (with the coin drop and position info needed) and `bossEscaped`. Coin spawning and the start of animations listen to these instead of being called directly by `Game`.
9. **Views read state.** The HUD (coins, damage, upgrade cost), the HP bar (length and colour) and the countdown read `GameState` when drawing. Remove all of `UI`'s setter calls from `Game`.
10. **Split `Animator`.** Replace it with `DeathAnimation` and `SmokeAnimation`, each with `start`, `update(dt)`, `draw` and `isFinished`. Fix the getters that return references to temporaries.
11. **Tidy `Game`.** No raw `new`/`delete`: owned objects become plain members or `std::unique_ptr`. `Game` should only own the pieces, run the loop, route input and wire listeners.

## Rules for whoever implements a step

- Implement **only** the current step, exactly as specified. If the spec is unclear or a design decision is needed, stop and report back rather than deciding.
- Never commit or push.
- Match the existing code style: tabs, PascalCase for types, camelCase for functions, parameters and locals, `m_` + camelCase for member variables (`m_hpMax`), no `this->`, and short `//` comments.
- C++17, SFML 3.1.0. Add new `.cpp` files to `add_executable` in `CMakeLists.txt`.
- Build with `cmake --build build --config Debug`, and leave the build with no new warnings.
