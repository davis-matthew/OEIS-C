/*
 * A300046  E.g.f. A(x) satisfies: A(x)^2 = 1 + Integral 2*A(4*x) dx.
 * https://oeis.org/A300046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, 2*a(n) + Σ_{k=1}^{n-1} C(n,k) a(k) a(n-k) =
 *            2*4^{n-1} a(n-1). (C(n,k) = binomial coefficient)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* holds a(0) .. a(m) */
static idx_t  cache_len  = 0;      /* allocated size of cache   */
static idx_t  cache_upto = 0;      /* last index computed      */
static seq_t  pow4_global = 1;     /* 4^cache_upto              */

/* Ensure cache can hold index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;
    size_t new_len = cache_len > 0 ? cache_len : 1;
    while (new_len <= n) new_len <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);      /* out of memory */
    cache = new_cache;
    cache_len = new_len;
}

/* Return a(n) for non‑negative n. */
seq_t a(idx_t n)
{
    /* First time: initialise cache and a(0). */
    if (cache == NULL) {
        ensure_cache(0);
        cache[0] = 1;
        cache_upto = 0;
        /* pow4_global already 1 (4^0) */
    }

    /* Extend cache up to n if necessary. */
    if (n > cache_upto) {
        ensure_cache(n);

        /* current pow4_global == 4^cache_upto
           which equals 4^(m-1) for the first new index m = cache_upto+1.   */
        for (idx_t m = cache_upto + 1; m <= n; ++m) {
            /* left = 4^(m-1) * a(m-1)  */
            seq_t left = pow4_global * cache[m - 1];
            seq_t rhs  = left * 2;                 /* 2*4^(m-1)*a(m-1) */

            /* sum_{k=1}^{m-1} C(m,k) a(k) a(m-k) */
            seq_t sum   = 0;
            seq_t binom = 1;                       /* C(m,0) */
            for (idx_t k = 1; k <= m - 1; ++k) {
                binom = binom * (m - k + 1);       /* C(m,k) numerator */
                binom = binom / k;                 /* exact division */
                seq_t term = binom * cache[k];
                term = term * cache[m - k];
                sum += term;
            }

            rhs -= sum;                            /* = 2*a(m)        */
            cache[m] = rhs / 2;                    /* integer division */

            pow4_global *= 4;                      /* now 4^m for next step */
        }
        cache_upto = n;
    }

    return cache[n];
}
