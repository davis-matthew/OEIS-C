/*
 * A390538  Numbers that do not exceed the square of their squarefree kernel.
 * https://oeis.org/A390538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that * k <= rad(k)^2,
 *            where rad(k) is the product of the distinct * primes dividing k
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether integer k satisfies k <= rad(k)^2. */
static bool passes(idx_t k)
{
    uint64_t rad = 1;
    uint64_t m = (uint64_t)k;
    uint64_t p;

    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            rad *= p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)            /* remaining prime factor */
        rad *= m;

    unsigned __int128 rad_sq = (unsigned __int128)rad * rad;
    return rad_sq >= k;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0; return 0 */

    idx_t count = 0;
    idx_t k = 1;

    for (;; ++k) {
        if (passes(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
