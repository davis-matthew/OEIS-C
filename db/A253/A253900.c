/*
 * A253900  a(n) is the number of squares of the form x^2 + x + n^2 for 0 <= x
 *          <= n^2.
 * https://oeis.org/A253900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of (4*n^2-1) divided by 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Count positive divisors of an unsigned 64‑bit integer m. */
static seq_t divisor_count_u64(uint64_t m)
{
    seq_t cnt = 1;
    uint64_t rem = m;

    /* 4*n^2-1 is always odd, skip factor 2. */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p != 0)
            continue;
        uint64_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }
        cnt *= (e + 1);
    }
    if (rem > 1)            /* the remaining factor is a prime */
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for n==0 for safety */
    if (n == 0)
        return 0;

    /* Compute m = 4*n^2 - 1. Use 128‑bit to avoid overflow. */
    unsigned __int128 t = (unsigned __int128)n * n;
    t = 4 * t - 1;
    uint64_t m = (uint64_t)t;  /* m fits in 64 bits for the tested range */

    seq_t tau = divisor_count_u64(m);
    return tau / 2;
}
