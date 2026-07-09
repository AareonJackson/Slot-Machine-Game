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

## Low Balance / Game Over
### Test: Manual spin with low balance
**Steps**
1. Reduce balance below current bet.
2. Click SPIN.

**Expected Result**
- [ ] Spin does not start.
- [ ] Balance does not go negative.
- [ ] Message appears: LOW BALANCE / Press RESET.
- [ ] RESET button remains usable.

### Test: Autoplay stops on low balance
**Steps**
1. Reduce balance near current bet.
2. Enable autoplay.
3. Let balance fall below bet.

**Expected Result**
- [ ] Autoplay stops.
- [ ] No negative balance occurs.
- [ ] Low-balance message appears.

### Test: Reset clears low-balance message
**Steps**
1. Trigger low-balance state.
2. Click RESET.

**Expected Result**
- [ ] Balance resets.
- [ ] Stats reset.
- [ ] Low-balance message disappears.

## Config Validation

### Test: Invalid bet values
**Steps**
1. Set defaultBet to a negative value.
2. Launch game.

**Expected Result**
- Game starts.
- Warning is printed.
- Bet uses a safe fallback value.

### Test: Invalid bet range
**Steps**
1. Set minBet higher than maxBet.
2. Launch game.

**Expected Result**
- Game starts.
- Warning is printed.
- Bet range is corrected.

### Test: Invalid spin timing
**Steps**
1. Set spinDurationSeconds too low.
2. Launch game and spin.

**Expected Result**
- Game starts.
- Warning is printed.
- Reels still stop before payout.

### Test: Invalid free spin settings
**Steps**
1. Set freeSpinTriggerSymbol to empty.
2. Set freeSpinTriggerCount to zero.
3. Set freeSpinsAwarded to negative.
4. Launch game.

**Expected Result**
- Game starts.
- Warnings are printed.
- Safe fallback free spin settings are used.