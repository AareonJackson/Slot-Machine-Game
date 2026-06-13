# Manual QA Checklist

## Before testing
- [ ] Build succeeds.
- [ ] Game launches.
- [ ] No missing asset errors.
- [ ] Save file state is known.

## Core gameplay
- [ ] Manual spin works.
- [ ] Bet is deducted.
- [ ] Reels stop one at a time.
- [ ] Win highlights appear.
- [ ] Win message appears.
- [ ] Balance updates correctly.

## Buttons
- [ ] SPIN works.
- [ ] BET + works.
- [ ] BET - works.
- [ ] AUTO toggles autoplay.
- [ ] HELP opens/closes.
- [ ] RESET resets progress.

## Free spins
- [ ] Valid trigger awards free spins.
- [ ] Scattered trigger symbols do not award free spins.
- [ ] Free spins do not deduct balance.
- [ ] Free spins update stats correctly.

## Save/load
- [ ] Save file is created.
- [ ] Balance persists.
- [ ] Stats persist.
- [ ] Reset persists.
- [ ] Missing save file works.
- [ ] Corrupt save file works.

## Final
- [ ] Commit changes after successful test pass.