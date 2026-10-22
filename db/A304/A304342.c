/*
 * A304342  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304342: Number of n×3 0..1 arrays with every element unequal to
   0, 1, 2, 4 or 5 king‑move adjacent elements, with upper left element zero.
   Offset 1 (a(1)=4).
   Empirical recurrence for n>16:
   a(n) =  2*a(n‑1) + 3*a(n‑2) + 6*a(n‑3) − 11*a(n‑4) − 25*a(n‑5) − 27*a(n‑6)
            + 19*a(n‑7) + 61*a(n‑8) + 59*a(n‑9) − 39*a(n‑11) − 13*a(n‑12)
            − 14*a(n‑13) − 20*a(n‑14) − 24*a(n‑15)  */

/* Standard headers required for all supported builds. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Initial 16 terms of the sequence (offset 1). */
static const seq_t init_vals[16] = {
    4ULL, 14ULL, 27ULL, 71ULL, 225ULL, 613ULL, 1752ULL, 5102ULL,
    14673ULL, 42235ULL, 122016ULL, 352304ULL, 1016793ULL,
    2936025ULL, 8478588ULL, 24482962ULL
};

/* Recurrence coefficients (k = 1 … 15, 0 for k = 10). */
static const int coeffs[15] = {
    2, 3, 6, -11, -25, -27, 19, 61, 59, 0,
    -39, -13, -14, -20, -24
};

/* Cached values of the sequence. */
static seq_t *cache = NULL;
/* Current capacity of the cache. */
static size_t cache_cap = 0;
/* Highest index computed so far (1‑based). */
static idx_t max_computed = 0;

/* Expand the cache to accommodate at least n elements.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap;
    while (new_cap < (size_t)n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a single term a(i) for i > 16 using the recurrence. */
static seq_t compute_term(idx_t i)
{
    seq_t pos = 0;
    seq_t neg = 0;
    for (int k = 1; k <= 15; ++k) {
        int c = coeffs[k - 1];
        idx_t idx = i - k;          /* 1‑based index of referenced term */
        seq_t t = cache[idx - 1];   /* cache stores a(1) at index 0 */
        if (c > 0)
            pos += (seq_t)c * t;
        else if (c < 0)
            neg += (seq_t)(-c) * t;
        /* c == 0 (k == 10) contributes nothing */
    }
    return pos - neg;
}

/* External linkage required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset 1: behaviour for n<1 is not defined */
        return 0;

    /* Initialise cache with the first 16 terms on first use. */
    if (max_computed == 0) {
        ensure_cache(16);
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        max_computed = 16;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    ensure_cache(n);
    for (idx_t i = max_computed + 1; i <= n; ++i)
        cache[i - 1] = compute_term(i);

    max_computed = n;
    return cache[n - 1];
}
