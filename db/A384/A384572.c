/*
 * A384572  Consecutive states of the linear congruential pseudo-random number
 *          generator (4219755981*s+1) mod 10^10 when started at s=1.
 * https://oeis.org/A384572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4219755981 * a(n-1) + 1) mod 10000000000, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Local helper to avoid overflow: compute (x * y) mod m using 128‑bit arithmetic. */
static seq_t mul_mod(seq_t x, seq_t y, seq_t m)
{
    unsigned __int128 prod = (unsigned __int128)x * (unsigned __int128)y;
    return (seq_t)(prod % m);
}

seq_t a(idx_t n)
{
    /* The recurrence is defined for n ≥ 1.  For safety, treat n == 0 as 0. */
    if (n == 0)
        return 0;

    const seq_t MOD = 10000000000ULL;   /* 10^10 */
    const seq_t MUL = 4219755981ULL;    /* multiplier */

    seq_t value = 1;                    /* a(1) */
    /* Compute iteratively up to a(n).  Complexity O(n), fast enough for n ≤ 10 000. */
    for (idx_t i = 1; i < n; ++i)
    {
        value = mul_mod(value, MUL, MOD);
        value += 1;
        if (value >= MOD)               /* avoid overflow of addition */
            value -= MOD;
    }
    return value;
}
