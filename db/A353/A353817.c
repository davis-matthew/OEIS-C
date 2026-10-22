/*
 * A353817  a(n) = 1 if n is a prime power with an odd exponent, otherwise 0.
 * https://oeis.org/A353817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a prime power with an odd exponent; otherwise 0
 * Range:     u8 n <= 9165, u16 n <= 9165, u32 n <= 9165, u64 n <= 9165,
 *            u128 n <= 9165, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Determine if n (idx_t) is a prime power with odd exponent.
   Uses only integer arithmetic and no external helper libraries. */
seq_t a(idx_t n)
{
    /* Handle offset. */
    if (n <= 1) return (seq_t)0;

    /* Work with uint64_t arithmetic to avoid overflow. */
    uint64_t v = (uint64_t)n;

    /* Find the smallest prime factor of v. */
    uint64_t p;
    if (v % 2ULL == 0ULL) {
        p = 2ULL;
    } else {
        p = v;          /* Assume v itself is prime. */
        for (uint64_t d = 3ULL; d * d <= v; d += 2ULL) {
            if (v % d == 0ULL) {
                p = d;   /* Found smallest prime factor. */
                break;
            }
        }
    }

    /* v is prime itself. */
    if (p == v) return (seq_t)1;

    /* Count the exponent of p in v. */
    uint64_t exponent = 0ULL;
    while (v % p == 0ULL) {
        v /= p;
        exponent++;
    }

    /* v should be 1 if it was a prime power; exponent must be odd. */
    if (v == 1ULL && (exponent & 1ULL) == 1ULL) {
        return (seq_t)1;
    } else {
        return (seq_t)0;
    }
}
