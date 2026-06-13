# Slot Machine Test Plan

## 1. Build and Launch

### Test: Debug build succeeds
**Steps**
1. Configure project with CMake.
2. Build Debug target.
3. Run `SlotMachine`.

**Expected Result**
- Build succeeds.
- Game launches without crashing.
- Assets load successfully.
- Main window appears.

---

## 2. Basic Spin Flow

### Test: Manual spin deducts bet
**Steps**
1. Start game with default balance.
2. Note balance and bet.
3. Click SPIN.

**Expected Result**
- Bet amount is deducted when spin starts.
- Reels animate.
- Reels stop one at a time.
- Final result displays.
- Balance updates after payout.

---

## 3. Bet Controls

### Test: Bet increase
**Steps**
1. Click BET +.

**Expected Result**
- Bet increases by configured bet step.
- Bet does not exceed max bet.
- Bet does not exceed balance.

### Test: Bet decrease
**Steps**
1. Click BET -.

**Expected Result**
- Bet decreases by configured bet step.
- Bet does not go below min bet.

---

## 4. Payouts

### Test: Winning spin
**Steps**
1. Spin until a winning payline appears.

**Expected Result**
- Winning cells highlight.
- Win amount appears.
- Balance increases by win amount.
- Session stats update.
- Win message appears.

---

## 5. Autoplay

### Test: Autoplay starts and stops
**Steps**
1. Click AUTO.
2. Let several spins run.
3. Click AUTO again.

**Expected Result**
- Autoplay starts repeated spins.
- Autoplay waits for each spin to finish before next spin.
- Autoplay stops when toggled off.

### Test: Autoplay stops on low balance
**Steps**
1. Reduce balance near bet amount.
2. Enable autoplay.

**Expected Result**
- Autoplay stops when balance is too low.
- No negative balance occurs.

---

## 6. Free Spins

### Test: Free spin trigger
**Steps**
1. Land the configured free spin trigger on a valid winning payline.

**Expected Result**
- Free spins are awarded.
- Free spin counter appears.
- Free spins start automatically.

### Test: Free spin does not deduct balance
**Steps**
1. Trigger free spins.
2. Watch next free spin.

**Expected Result**
- Balance is not reduced at free spin start.
- Wins during free spins still increase balance.

### Test: Scattered trigger symbols do not trigger
**Steps**
1. Land trigger symbols not on a valid winning payline.

**Expected Result**
- Free spins are not awarded.

---

## 7. Save/Load

### Test: Save after spin
**Steps**
1. Spin once.
2. Close game.
3. Reopen game.

**Expected Result**
- Balance and stats persist.

### Test: Reset progress
**Steps**
1. Change balance/stats by spinning.
2. Click RESET.
3. Close and reopen game.

**Expected Result**
- Balance and stats remain reset.

### Test: Missing save file
**Steps**
1. Delete save file.
2. Launch game.

**Expected Result**
- Game starts with defaults.
- No crash.

### Test: Corrupt save file
**Steps**
1. Replace save file contents with invalid JSON.
2. Launch game.

**Expected Result**
- Game starts with defaults.
- Error is logged.
- No crash.

---

## 8. Help/Paytable

### Test: Help overlay
**Steps**
1. Click HELP.
2. Click HELP again.

**Expected Result**
- Help overlay opens.
- Help overlay closes.
- Game still functions afterward.

---

## 9. Reset

### Test: Reset while idle
**Steps**
1. Spin several times.
2. Click RESET.

**Expected Result**
- Balance resets.
- Stats reset.
- Free spins clear.
- Autoplay stops.
- Save file updates.

### Test: Reset while spinning
**Steps**
1. Start a spin.
2. Click RESET during animation.

**Expected Result**
- Reset is ignored until spin completes, or safely prevented.
- No crash.