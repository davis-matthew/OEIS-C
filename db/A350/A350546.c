/*
 * A350546  Numbers k such that the period of the continued fraction for
 *          sqrt(k) is a prime.
 * https://oeis.org/A350546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer k > 1 such that the period of the
 *            continued fraction for sqrt(k) is a prime number
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Integer square‑root (floor) using binary search with 128‑bit math.   */
/* --------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;          /* (2^32)^2 > 2^64-1 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/* Simple deterministic primality test for unsigned 32‑bit integers.      */
/* --------------------------------------------------------------------- */
static inline bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int i = 3; (unsigned long long)i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/* Global cache for computed terms.  The cache grows as needed.         */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap  = 0;

/* --------------------------------------------------------------------- */
/* Compute the nth term (n >= 1).  The function can be called in any    */
/* order; it remembers previously found terms and only expands the     */
/* search when more terms are required.                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 */

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* The next candidate integer to test */
        static idx_t next_k = 2;             /* start after 1 */
        idx_t k = ++next_k;

        /* Skip perfect squares */
        idx_t sq = isqrt_u64(k);
        if (sq * sq == k) continue;

        /* Compute period length of continued fraction of sqrt(k) */
        idx_t a0 = sq;
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;
        idx_t period = 0;

        while (1) {
            m = d * a - m;
            d = (k - m * m) / d;
            a = (a0 + m) / d;
            period++;

            if (a == 2 * a0) break;          /* period finished */
        }

        /* If period is prime, store k */
        if (is_prime_small((unsigned int)period)) {
            if (cache_size == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(1); } /* OOM */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
