# Manual QA Checklist

## Before testing
- [x] Build succeeds.
- [x] Game launches.
- [x] No missing asset errors.
- [x] Save file state is known.

## Core gameplay
- [x] Manual spin works.
- [x] Bet is deducted.
- [x] Reels stop one at a time.
- [x] Win highlights appear.
- [x] Win message appears.
- [x] Balance updates correctly.

## Buttons
- [x] SPIN works.
- [x] BET + works.
- [x] BET - works.
- [x] AUTO toggles autoplay.
- [x] HELP opens/closes.
- [x] RESET resets progress.

## Free spins
- [x] Valid trigger awards free spins.
- [x] Scattered trigger symbols do not award free spins.
- [x] Free spins do not deduct balance.
- [x] Free spins update stats correctly.

## Save/load
- [x] Save file is created.
- [x] Balance persists.
- [x] Stats persist.
- [x] Reset persists.
- [x] Missing save file works.
- [x] Corrupt save file works.

## Final
- [x] Commit changes after a successful test passes.