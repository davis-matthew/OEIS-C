/*
 * A377524  Number of steps for n to reach the minimum of its final cycle
 *          under iterations of the map (A123684): x->(3x-1)/2 if x odd, x/2
 *          otherwise; or -1 if this never happens.
 * https://oeis.org/A377524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of steps for n to reach the minimum of its final cycle
   under the iteration:  f(x) = (3x-1)/2  if x odd, otherwise x/2.
   Uses Floyd’s cycle‑finding algorithm and determines the minimal value
   in the final cycle. */

static inline seq_t f(seq_t x) {
    return (x & 1) ? (seq_t)((seq_t)3 * x - 1u) / 2 : x / 2;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)-1;          /* safety for out‑of‑range input */

    seq_t start   = (seq_t)n;
    seq_t tortoise = f(start);
    seq_t hare     = f(f(start));

    /* ----- Floyd’s cycle detection ----- */
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare     = f(f(hare));
    }

    /* ----- Find the start of the cycle ----- */
    seq_t slow = start;
    idx_t startSteps = 0;
    while (slow != tortoise) {
        slow = f(slow);
        tortoise = f(tortoise);
        ++startSteps;
    }

    /* ----- Determine cycle length ----- */
    idx_t cycleLen = 1;
    seq_t cur = f(slow);
    while (cur != slow) {
        cur = f(cur);
        ++cycleLen;
    }

    /* ----- Find minimal value in the cycle ----- */
    seq_t minVal   = slow;
    idx_t posInCycle = 0;
    cur = slow;
    for (idx_t k = 1; k < cycleLen; ++k) {
        cur = f(cur);
        if (cur < minVal) {
            minVal = cur;
            posInCycle = k;
        }
    }

    return (seq_t)(startSteps + posInCycle);
}
