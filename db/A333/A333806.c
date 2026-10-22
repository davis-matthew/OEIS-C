/*
 * A333806  Number of distinct prime divisors of n that are < sqrt(n).
 * https://oeis.org/A333806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors p of n with p < sqrt(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be computed by factoring n and counting
   all distinct prime factors whose square is strictly less than n.
   Since n <= 2^64-1 we can safely use a 128‑bit intermediate for
   the inequality. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Use a buffer large enough for all prime factors up to 64‐bit n.
       For 64‑bit n the number of distinct prime factors is at most 64,
       but 128 gives a margin. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t count = 0;
    const uint64_t nn = (uint64_t)n; /* preserve for 128‑bit computation */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* Count p if p*p < n  (strictly less). */
        if ((unsigned __int128)p * p < (unsigned __int128)nn)
            ++count;
    }

    return count;
}
