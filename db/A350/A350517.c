/*
 * A350517  a(n) is the least prime p such that p^2+n = q*(n+1) for some prime
 *          q.
 * https://oeis.org/A350517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 25, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search definition:
   a(n) is the least prime p such that p^2 + n = q * (n + 1) for some prime q.
   This implementation searches for the smallest prime ≥ ceil(sqrt(n+2))
   and tests the divisibility condition until a suitable q is found. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

/* Integer floor square root for 64‑bit values (binary search). */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;                 /* 2^32 > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid != 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* lower bound for p: ceil(sqrt(n+2)) */
    idx_t lower = floor_sqrt_u64(n + 2);
    if (lower * lower < n + 2)
        ++lower;

    uint64_t p = lower;
    for (;;) {
        /* Skip to the next prime if p is not prime. */
        if (!is_prime_small((seq_t)p))
            p = next_prime(p);

        /* Compute p^2 + n and test divisibility by n+1. */
        seq_t pp  = (seq_t)p * (seq_t)p;   /* p² */
        seq_t val = pp + (seq_t)n;         /* p² + n */
        seq_t denom = (seq_t)(n + 1);      /* n + 1 */

        if (val % denom == 0) {
            seq_t q = val / denom;
            if (is_prime_big(q))
                return (seq_t)p;          /* found the least prime */
        }

        /* Try the next prime. */
        p = next_prime(p);
    }
}
