/*
 * A391208  Expansion of 1/(g * (2-g)), where g = 1+x*g^3 is the g.f. of
 *          A001764.
 * https://oeis.org/A391208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0 a(n) = (2/n) * Σ_{k=0}^{n-1} k *
 *            binomial(3*n-2, n-1-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the binomial routine */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Cache for previously computed terms */
static seq_t *cache = NULL;       /* array of a(i) */
static idx_t cache_cap = 0;       /* allocated capacity */
static idx_t cache_count = 0;     /* number of stored values (highest+1) */

/* compute a single value */
static seq_t compute_value(idx_t n)
{
    if (n == 0)    /* a(0) = 1 */
        return 1;

    /* constants for the summation */
    idx_t m = 3 * n - 2;   /* a choose */
    idx_t r = n - 1;       /* current lower index */

    /* We must handle two cases: when seq_t is small (≤8 bytes) we use
       unsigned __int128 for intermediate arithmetic; otherwise we
       rely on the arbitrary‑precision binomial routine. */
    if (sizeof(seq_t) <= 8) {
        /* n small enough that all intermediate values fit in 128 bits */
        unsigned __int128 binom = 1;
        /* compute C(m, r) multiplicatively */
        for (idx_t i = 1; i <= r; ++i) {
            binom = (binom * (unsigned __int128)(m - r + i)) / (unsigned __int128)i;
        }

        unsigned __int128 sum = 0;
        for (idx_t k = 0; k < n; ++k) {
            unsigned __int128 term = ((unsigned __int128)k) * binom;
            sum += term;
            if (k + 1 < n) {
                idx_t old_r = r;
                r = r - 1;
                /* C(m, r-1) = C(m, old_r) * old_r / (m - old_r + 1) */
                binom = (binom * (unsigned __int128)old_r) /
                        (unsigned __int128)(m - old_r + 1);
            }
        }
        unsigned __int128 num = sum * 2;
        unsigned __int128 res = num / n;
        return (seq_t)res;
    } else {
        /* full‑precision arithmetic using the provided binomial routine */
        seq_t binom = binomial(m, r);
        seq_t sum = 0;
        for (idx_t k = 0; k < n; ++k) {
            seq_t term = ((seq_t)k) * binom;
            sum += term;
            if (k + 1 < n) {
                idx_t old_r = r;
                r = r - 1;
                /* exact division guaranteed */
                binom = (binom * (seq_t)old_r) / (seq_t)(m - old_r + 1);
            }
        }
        seq_t num = sum * 2;
        seq_t res = num / n;
        return res;
    }
}

/* public entry point with caching */
seq_t a(idx_t n)
{
    /* Initialise cache lazily */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;
        cache[0] = 1;          /* a(0) */
        cache_count = 1;
    }

    /* Grow array if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n + 1 > cache_count) {
        for (idx_t i = cache_count; i <= n; ++i)
            cache[i] = compute_value(i);
        cache_count = n + 1;
    }

    return cache[n];
}
