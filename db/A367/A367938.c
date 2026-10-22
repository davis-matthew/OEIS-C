/*
 * A367938  Expansion of e.g.f. exp(exp(3*x) - 1 - 2*x).
 * https://oeis.org/A367938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = -2*a(n-1) + Σ_{k=1..n}
 *            C(n‑1,k‑1)*3^k*a(n‑k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   grow_buffer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- includes -------------------------------------------------------- */
#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc, free */
#include "../utils/grow_buffer.h"

/* --- global cache ---------------------------------------------- */
static seq_t *cache = NULL;      /* computed a(n) values            */
static size_t cache_cap = 0;     /* allocated size of cache        */
static idx_t  cache_max = 0;     /* highest n computed (>=0)       */

static seq_t *pow3 = NULL;       /* powers of 3: pow3[i] = 3^i     */
static size_t pow3_cap = 0;      /* allocated size of pow3         */
static idx_t  pow3_max = 0;      /* highest index pre‑computed    */

/* --- power of 3 cache ------------------------------------------------- */
static void ensure_pow3(idx_t n) {
    if (pow3 == NULL) {                   /* first allocation */
        pow3_cap = 1;
        pow3 = (seq_t *)malloc(pow3_cap * sizeof(seq_t));
        pow3[0] = (seq_t)1;                /* 3^0 = 1 */
        pow3_max = 0;
    }
    if (pow3_cap < (size_t)(n + 1)) {
        size_t new_cap = pow3_cap * 2;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        pow3_cap = new_cap;
    }
    for (idx_t i = pow3_max + 1; i <= n; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow3_max = n;
}

/* --- sequence cache --------------------------------------------------- */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {                  /* first allocation */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        cache_max = 0;
    }
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* --- main recurrence computation ------------------------------------- */
static void compute_upto(idx_t n) {
    ensure_cache(n);
    ensure_pow3(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                /* C(i‑1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * 3^k * a(i‑k) */
            seq_t term = binom;
            term *= pow3[k];
            term *= cache[i - k];
            sum += term;
            if (k < i) {
                /* update binom to C(i‑1, k) */
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k);
            }
        }
        cache[i] = sum - ((seq_t)2 * cache[i - 1]);
    }
    cache_max = n;
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    compute_upto(n);
    return cache[n];
}
