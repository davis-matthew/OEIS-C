/*
 * A384081  Consecutive internal states of a linear congruential pseudo-random
 *          number generator for the Hewlett-Packard HP-20S when started at 1.
 * https://oeis.org/A384081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = (997 * a(n‑1)) mod 10^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* linear‑congruential recurrence for the HP‑20S */
seq_t a(idx_t n)
{
    if (n == 0)    /* undefined for n < 1, but return 0 to be safe */
        return (seq_t)0;

    /* 10^7 fits easily in 64 bits */
    const unsigned long long MOD = 10000000ULL;

    unsigned long long result = 1ULL;   /* a(1) */
    unsigned long long base   = 997ULL % MOD;
    idx_t exp = n - 1;

    /* fast exponentiation: a(n) = base^(n-1) mod MOD */
    while (exp > 0) {
        if (exp & 1U)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    return (seq_t)result;
}
