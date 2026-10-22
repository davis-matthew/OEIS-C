/*
 * A303946  Numbers that are neither squarefree nor perfect powers.
 * https://oeis.org/A303946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, grow_seq, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are neither squarefree nor perfect powers.
   a(n) is the n‑th positive integer that is not squarefree
   (has a squared prime factor) and is not a perfect power. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/grow_seq.h"
#include "../utils/gcd_uint64.h"

/* Helper: gcd for unsigned 64‑bit values. */

/* Test if x is a perfect power.
   For small x (≤ 2⁶⁴) we factor it and check whether the
   greatest common divisor of the prime exponents is > 1. */
static bool is_perfect_power(seq_t x)
{
    if (x < 4) return false;        /* 1,2,3 are not powers of degree ≥ 2 */
    uint64_t n = (uint64_t)x;        /* candidate value fits in 64 bits
                                       for the sizes we test. */
    uint64_t temp = n;
    uint64_t g = 0;                  /* gcd of exponents */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (g == 0)
                g = exp;
            else
                g = gcd_uint64(g, exp);
            if (g == 1)          /* cannot be a perfect power any more */
                return false;
        }
    }
    if (temp > 1) {                /* remaining prime factor */
        if (g == 0)                /* prime alone → exponent 1 */
            g = 1;
        else
            g = gcd_uint64(g, 1);   /* gcd(g,1) = 1 → not a power */
    }
    return g > 1;
}

/* ------------------------------------------------------------------ */
/* Cached sequence values – supports out‑of‑order calls                */
static seq_t *cache = NULL;     /* array of stored terms                         */
static size_t cache_cap = 0;    /* allocated capacity                           */
static size_t cache_len = 0;    /* number of terms actually stored              */
static seq_t next_candidate = 2;/* next integer to examine                      */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, but safe */

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        /* Find next integer that satisfies the property. */
        seq_t cand = next_candidate;
        while (true) {
            if (!is_squarefree(cand) && !is_perfect_power(cand))
                break;          /* candidate found */
            cand++;
        }

        /* Grow cache capacity if needed. */
        if (cache_cap < cache_len + 1) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            if (newcap < cache_len + 1)
                newcap = cache_len + 1;
            grow_seq(&cache, &cache_cap, newcap);
        }

        cache[cache_len++] = cand;
        next_candidate = cand + 1;   /* advance for next search */
    }

    /* Return the requested term. */
    return cache[n - 1];
}
