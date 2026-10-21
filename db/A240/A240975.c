/*
 * A240975  The number of distinct prime factors of n^3-1.
 * https://oeis.org/A240975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of distinct prime factors of n^3 - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1, a(1) = 0 because 0 has no prime factors. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  /* provides factor_u64() */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t v = (uint64_t)n;
    /* Compute v^3 - 1 safely using 128-bit intermediate. */
    unsigned __int128 cube = (unsigned __int128)v * v * v;
    cube -= 1;
    uint64_t num = (uint64_t)cube;   /* n^3 - 1 fits in 64 bits for n <= 10000 */

    /* Factorization buffers. Number of distinct primes for
       numbers up to 1e12 is at most 8, so 20 slots is safe. */
    uint64_t pr[20], ex[20];
    size_t cnt = 0;

    factor_u64(num, pr, ex, &cnt);

    return (seq_t)cnt;
}
