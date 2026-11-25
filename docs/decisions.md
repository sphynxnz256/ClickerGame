# Decision log

Design decisions for ClickerGame: what was chosen, the alternatives, why, and my own thinking when I gave it. Newest entries go at the bottom.

## SFML 3.1 before the refactor (2026-11-04)
- **Chose:** Upgrade SFML 2.6.2 → 3.1.0 first, as its own PR, then refactor.
- **Instead of:** Refactoring first, or doing both at once.
- **Why:** The upgrade is mechanical and easy to verify against the known-working structure. SFML 3's rules, such as a sprite needing a texture when it's created, shape how classes should be designed, so designing classes before the upgrade would have meant doing it twice.

## Naming style (2026-11-13)
- **Chose:** PascalCase types, camelCase functions/parameters/locals, `m_` + camelCase members, no `this->`.
- **Instead of:** The old tutorial style of snake_case parameters and `this->` everywhere.
- **Why:** One consistent rule. `m_` makes parameter/member name clashes impossible, while `this->` only works if you never forget it. SFML's own source uses `m_`.
- **My thinking:** I don't normally use snake_case in C++. Some came from YouTube tutorials I followed about 4 years ago, and the rest from an earlier AI session during the SFML 3 migration. camelCase everywhere is my own preference. I only used `this->` because I thought it was best practice, so if the `m_` prefix is better, I'd rather use that.

## Architecture: state / content / rules / view / core (2026-11-13)
- **Chose:** Plain-data state as the single source of truth, content as data, rules as plain C++ with no SFML, and views that read state.
- **Instead of:** One file per action (Unity style), full ECS, or a global event bus.
- **Why:** Each piece of data has one owner, so nothing needs syncing by hand. The rules can be read and tested on their own. Grouping by responsibility suits a game with roughly one action. ECS and a global event bus are overkill at this size.
- **My thinking:** I gave up on this project years ago because of tight coupling. A more recent Unity project felt much better designed: each entity had one stats file as its single source of truth, small separate files handled each action, and things announced events, like an enemy taking damage. I wanted a similar idea here, checked against real game-dev best practice. Best practice but minimal, not over-engineered, and no YouTube or beginner-guide shortcuts. In a bigger game I'd expect separate player, enemy and game state files to make more sense than one big file. Once I understood that this design does keep them separate, the recommendation made sense to me.

## Enemies as data (Type Object) (2026-11-13)
- **Chose:** One `Enemy` with `EnemyType` data. The regular enemy is one type with 5 textures picked at random, and the boss is a second type.
- **Instead of:** Separate `Enemy`/`Boss` classes, or inheritance.
- **Why:** With two classes, every change had to be made twice. A new enemy should be new data, not a new class.
- **My thinking:** I used inheritance for the boss and enemy, and changing one or the other got messy. The regular enemy really is one enemy: the different sprites are only for variety, with the same HP and coin drops. I don't plan to change that unless enemies start attacking, which would add a lot of complexity. I'd rather focus on levels and the upgrade system.

## Content in C++, not JSON (2026-11-13)
- **Chose:** Enemy definitions as C++ constants in `content/`.
- **Instead of:** JSON files loaded at startup.
- **Why:** No library is needed and the compiler checks the data. Revisit when levels add a lot of content.
- **My thinking:** The recommendation works for now. We can consider JSON files if the game gets bigger.

## Levels left out of the refactor (2026-11-13)
- **Chose:** No level system during the refactor. Content still moved into data so levels slot in later.
- **Instead of:** Building levels as part of the refactor.
- **Why:** The refactor's test is "plays exactly the same". Levels also need progression decisions that haven't been made yet.
- **My thinking:** In my mind the refactor was always going to be done without levels. Levels will matter long term, so I wanted to check, but I prefer leaving them for later anyway.

## One frame clock (2026-11-13)
- **Chose:** `Game` measures `dt` once per frame as a `float` of seconds and passes it down. No cap on long frames.
- **Instead of:** A clock in each class, or `sf::Time`, or capping `dt`.
- **Why:** Every part of the game agrees on frame time, which removes the "first frame" workarounds. A cap would change behaviour and can be added later on its own.

## Clicks from events (2026-11-13)
- **Chose:** The SFML "mouse button pressed" event, acting on press, routed first-match-wins in the order buttons → coins → enemy. One click buys one upgrade. Retreat only works while it's visible.
- **Instead of:** Polling whether the button is held, with `mouseHeld` flags. Also considered: acting on release, or giving the click to everything under the mouse.
- **Why:** Each click is handled exactly once, in one place. Hold-to-repeat upgrades and the invisible Retreat button were bugs.
- **My thinking:** I suspected `mouseHeld` and the click flags were relics from using an old SFML version. I didn't realise the button bug existed, and I wanted it fixed.

## Texture cache owned by Game (2026-11-13)
- **Chose:** One `TextureCache` owned by `Game` and passed by reference, loading each texture on first request.
- **Instead of:** A global or singleton, or preloading everything.
- **Why:** Dependencies are visible in constructors. A singleton is hidden coupling. Preloading isn't needed at this size.

## One current enemy, boss flag on the type (2026-11-13)
- **Chose:** A single current enemy whose type switches to the boss and back. Boss-ness is `EnemyType::isBoss`. Game phase is `enum class Phase { Fighting, Dying, Escaping }`.
- **Instead of:** Separate enemy and boss objects, and true/false flags.
- **Why:** A flag can disagree with what's on screen. The type can't. One `Phase` can't be in two states at once.

## Big numbers as double (2026-11-13)
- **Chose:** Coins, damage, HP and costs are `double`. On-screen numbers go through `formatNumber()`, which shows whole numbers for now.
- **Instead of:** `int`, or adding K/M/B suffixes now.
- **Why:** An incremental game passes `int`'s limit of ~2.1 billion. Formatting in one place means a suffix style later is a one-function change.
- **My thinking:** I chose whole numbers for now (option A), because my intent is to keep the game looking the same during the refactor.

## Rules as free functions on the smallest state (2026-11-13)
- **Chose:** Free functions in `namespace rules`, each taking only the state it needs, e.g. `clickDamage(const PlayerState&)`. The boss timer lives in `GameState`, with the time limit on the boss's type.
- **Instead of:** Rule classes, or the timer living in `UI`.
- **Why:** Rules have no data of their own. Narrow parameters show what a rule can and can't touch. The UI should display state, not own it.

## Events deferred (2026-11-16)
- **Chose:** Keep `Game::onEnemyKilled()` as a direct call.
- **Instead of:** A `Signal` listener list for "enemy died" and "boss escaped".
- **Why:** Both reactions live in `Game`, so an event adds indirection without removing coupling. Add events when a second, independent system needs to react, such as sound, stats, achievements or level progress.
- **My thinking:** Agreed: leave events for now.

## Views read state every frame; UI split by job (2026-11-16)
- **Chose:** `HudView`, `HpBarView` and `BossCountdownView` read `GameState` each frame and rebuild their text.
- **Instead of:** One `UI` class with setters that `Game` calls, or only updating text when values change.
- **Why:** The screen can't go stale. SFML already skips rebuilding text when the string is unchanged, so the cost is negligible. Each view has one job and one file.
- **My thinking:** I prefer one file per job, if it isn't over-engineering, and the UI file had got quite long. I was worried that updating text every frame was inefficient. Once I understood that SFML doesn't rebuild text that hasn't changed, rebuilding every frame was the simpler choice.

## Animator split into DeathAnimation and SmokeAnimation (2026-11-16)
- **Chose:** Two classes in `view/`. `DeathAnimation::start(sprite)` copies the enemy's sprite and shrinks its own copy. `SmokeAnimation::start(centre)` takes its position when it starts.
- **Instead of:** Keeping a pointer to the live enemy sprite, or positioning the smoke once at startup.
- **Why:** No raw pointer, and the animation can't leave another object's sprite at scale 0. A sprite copy is cheap, because it only refers to its texture. The smoke doesn't need to know about the window, and can appear elsewhere later.
- **My thinking:** Agreed with both recommendations.

## Game owns its parts as plain members; missing font fails fast (2026-11-16)
- **Chose:** `Game` holds the window, enemy view, font and views as plain members built in the constructor's init list, with no destructor. A missing `arial.ttf` throws, and `main` catches it, prints a clear message and exits.
- **Instead of:** `std::unique_ptr` members created in `init` functions, or printing an error and carrying on with a broken font.
- **Why:** The parts always exist and are never replaced, so a pointer adds nothing. Plain members clean themselves up in reverse declaration order (the rule of zero). The game is unplayable without its font, so failing at startup is clearer than blank text.
- **My thinking:** I want it to fail fast so I can fix the bug, not keep working with the font missing.

## First enemy comes from the state (2026-11-16)
- **Chose:** `rules::startingState()` returns a state with the first enemy already spawned. `Game` builds its enemy view from that state's enemy type.
- **Instead of:** Naming `content::regularEnemy` separately for the view and for the state, or making the sprite optional.
- **Why:** One source for the first enemy, so the view and the state can't disagree. It also removes the throwaway second texture pick at startup.
- **My thinking:** Picking a texture twice looked like bad design to me, so I chose to fix it, not leave it as a minor wart.

## Coins collected by mouse-over, after a short delay (2026-11-17)
- **Chose:** Each frame, `updateCoins` moves each coin and collects it if it is collectable and the cursor is over it. A coin becomes collectable a fixed time after it spawns (`m_pickupDelay`, a private `Coin` member set in `initVariables()`), so it can be caught in flight. The mouse position is polled once per frame in `update()`.
- **Instead of:** Collecting on click, `MouseMoved` events, a separate pass over the coins, collecting in the draw loop, a spatial grid, or letting a coin be collected the moment it spawns.
- **Why:** Polling also collects a coin that lands under a still mouse, which events would miss. The coin loop already visits every coin each frame, so the hover check adds no extra pass; a grid wouldn't help while coins move and there are only tens of them. The player holds the mouse on the enemy's centre, which is where coins spawn, so without a delay every coin would be collected instantly. The delay is a named tuning value, not a hard-coded number.
- **My thinking:** I want to catch coins in flight, but not instantly at the spawn point. If the delay can't be made to feel right, drop it and make coins collectable only once landed.

## Coins fly as projectiles with a random launch (2026-11-25)
- **Chose:** Each coin gets a random launch speed (450-650 px/s) and angle (45-135 degrees) and is pulled down by gravity (2200 px/s^2). Its position is a formula of elapsed time: x = startX + vx * t and y = startY + vy * t + 0.5 * g * t^2. It stops on the floor line, and where it lands is a result of the launch.
- **Instead of:** The quadratic Bezier curve to a random landing spot, a Bezier with a corrected control point, per-frame physics that adds velocity each frame, or choosing the landing spot first.
- **Why:** The sideways movement has one direction, so the path can never curve back on itself. The ranges were checked over every whole-number speed and angle with the 800x600 window, the enemy centred and a coin 56 px wide: the farthest landing is 304 px from the centre against 352 px of room, the highest rise is 96 px, and flights last 0.57-0.79 s. Tuned from a first set (350-550 px/s, 30-150 degrees, 1800 px/s^2) after play-testing, to get a higher arc and a slightly quicker flight. Choosing the launch (not the landing spot) gives different arcs for different coins. No clamp is needed, and there is no bounce.
- **My thinking:** Forcing a landing spot might not look good with projectile motion, so I chose to randomise the launch angle and speed instead, with ranges picked so coins never leave the screen or hit its sides. If the window size, the enemy position or the coin size changes, the ranges need rechecking.
