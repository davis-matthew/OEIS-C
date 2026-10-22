/*
 * A385460  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (1366*s + 150889) mod 714025 when
 *          started at 1.
 * https://oeis.org/A385460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (1366*a(n-1) + 150889) mod 714025
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t MULT = 1366;
    const seq_t INC  = 150889;
    const seq_t MOD  = 714025;
    const idx_t PERIOD = 714025;      /* number of distinct states */

    if (n == 0) {
        return 0;   /* undefined by problem statement, guard against underflow */
    }

    /* reduce the index modulo the period; a(1) corresponds to residue 0 */
    idx_t steps = (n - 1) % PERIOD;

    seq_t x = 1; /* a(1) */

    for (idx_t i = 0; i < steps; ++i) {
        x = (MULT * x + INC) % MOD;
    }

    return x;
}
