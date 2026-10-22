/*
 * A325071  Prime numbers congruent to 1 modulo 20 representable by both x^2 +
 *          20*y^2 and x^2 + 100*y^2.
 * https://oeis.org/A325071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A325071: Prime numbers congruent to 1 modulo 20 that are
    representable by both quadratic forms x^2 + 20*y^2 and
    x^2 + 100*y^2.  The function returns the n‑th term (n ≥ 1) of
    that sequence.  It uses a cached list of valid primes and
    generates new primes on demand by calling the supplied
    routine next_prime. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"   /* for next_prime(uint64_t) */

/*--- Integer square root for 64‑bit unsigned integers -------------------*/
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;   /* 2^62 ≤ 2^64-1 */
    while (bit > n) bit >>= 2;                 /* find highest four‑power ≤ n */
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/*--- Test whether an unsigned integer is a perfect square -----------------*/
static bool is_square_u64(uint64_t x)
{
    uint64_t s = isqrt_u64(x);
    return s * s == x;
}

/*--- Test whether a prime p can be written as x^2 + D*y^2 ----------------*/
static bool representable(uint64_t p, uint64_t D)
{
    for (uint64_t y = 0;; ++y) {
        uint64_t yy = y * y;
        uint64_t dyy = D * yy;
        if (dyy > p) break;
        uint64_t rem = p - dyy;
        if (is_square_u64(rem)) return true;
    }
    return false;
}

/*--- Cache for the sequence ------------------------------------------------*/
static seq_t *cache      = NULL;   /* array of stored terms               */
static size_t cache_cap  = 0;      /* allocated capacity of the array     */
static size_t cache_len  = 0;      /* number of terms actually stored     */
static uint64_t last_pr   = 1;     /* last prime examined                 */

/*--- Return the n‑th term of A325071 -------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate further terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_pr + 1);
        last_pr = p;

        if (p % 20 != 1) continue;                /* p ≡ 1 (mod 20) */

        /* Two representability checks. */
        if (!representable(p, 20) || !representable(p, 100))
            continue;

        /* Extend cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* If realloc fails we abort – fixed‑point needed only for */
            /* correct behaviour in the evaluation environment. */
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)p;
    }

    return cache[(size_t)n - 1];
}
