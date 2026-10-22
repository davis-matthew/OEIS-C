/*
 * A374192  Number of iterations required to reach 1 or 10 in a modified
 *          Collatz trajectory where x -> x/2 if x is even, x -> x+1 if x is
 *          odd and not divisible by 3 and x -> 3x+1 if x is odd and divisible
 *          by 3; or a(n) = -1 if 1 or 10 is never reached.
 * https://oeis.org/A374192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations to reach 1 or 10 in the modified
 *            Collatz trajectory (x even → x/2; x odd & divisible by 3 → 3x+1;
 *            x odd & not divisible by 3 → x+1). Returns (seq_t)-1 if the
 *            iteration never reaches 1 or 10
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start value */
    seq_t x = (seq_t)n;
    /* Special cases */
    if (x == (seq_t)1ULL || x == (seq_t)10ULL)
        return (seq_t)0;

    /* Count steps using a 64‑bit counter to avoid overflow of seq_t */
    idx_t steps = 0;
    const idx_t MAX_STEPS = 20000000U;   /* safety bound */

    while (steps < MAX_STEPS) {
        /* Next value according to the rules */
        if ((x & (seq_t)1ULL) == (seq_t)0ULL) { /* even */
            x >>= 1;               /* x = x / 2 */
        } else {
            if ((x % (seq_t)3ULL) == (seq_t)0ULL) {   /* odd and divisible by 3 */
                x = x * (seq_t)3 + (seq_t)1;   /* x = 3x + 1 */
            } else {                              /* odd and not divisible by 3 */
                x = x + (seq_t)1;                /* x = x + 1 */
            }
        }
        ++steps;

        if (x == (seq_t)1ULL || x == (seq_t)10ULL)
            return (seq_t)steps;
    }

    /* If the bound was exceeded, assume the cycle never hit 1 or 10. */
    return (seq_t)-1;
}
