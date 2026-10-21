/*
 * A276632  Integer part of the geometric mean of the prime factors of n.
 * https://oeis.org/A276632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (rad(n))^(1 / ω(n)) ) where rad(n) is the product
 *            of distinct prime factors of n and ω(n) is the number of
 *            distinct prime factors
 * Range:     u8 n <= 32, u16 n <= 4405, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

/* Compute floor of k‑th root of x (x >= 1, k >= 1). */
static seq_t kth_root(seq_t x, seq_t k)
{
    if (k == 1) return x;
    seq_t low = 1;
    seq_t high = x;
    seq_t ans = 1;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t p = big_pow(mid, k);          /* mid^k */
        if (p <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            /* if mid^k > x, any larger mid will also exceed */
            if (mid == 0) break;            /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* special case: 1 has no prime factors */

    seq_t rad = (seq_t)1;                  /* product of distinct primes */
    seq_t k   = (seq_t)0;                  /* number of distinct primes */
    idx_t m   = n;

    for (idx_t p = 2; p <= m / p; ++p) {   /* trial division up to sqrt(m) */
        if (m % p == 0) {
            rad *= (seq_t)p;
            ++k;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {                           /* remaining prime factor > sqrt(original) */
        rad *= (seq_t)m;
        ++k;
    }

    /* At this point rad >= 2 and k >= 1 for n > 1 */
    return kth_root(rad, k);
}
