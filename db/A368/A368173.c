/*
 * A368173  Expansion of e.g.f. -log(1 - x^2/2 * (exp(x) - 1)).
 * https://oeis.org/A368173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 + Sum_{k=3}^{n-1} k*(k-1)/2 * binomial(n-1,k) *
 *            a(n-k) with a(0) = a(1) = a(2) = 0. Uses an explicit cache to
 *            compute terms on demand; the cache grows as needed. The
 *            implementation is valid C11 and C++17
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

#ifdef __cplusplus
/* C++ build: use std::vector<seq_t> */
static std::vector<seq_t> cache = { (seq_t)0, (seq_t)0, (seq_t)0 };
static idx_t max_cached = 2;   /* indices 0..2 already initialized */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;

    /* grow storage if needed */
    if ((idx_t)cache.size() <= n)
        cache.resize((size_t)n + 1);

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)0;     /* a(0)=a(1)=a(2)=0 */
            continue;
        }

        seq_t term1 = (seq_t)i * ((seq_t)i - 1) / 2; /* n*(n-1)/2 */
        seq_t sum   = 0;
        seq_t binom = (seq_t)1;                 /* C(n-1,0) */

        for (idx_t k = 1; k < i; ++k) {
            /* binom = C(n-1, k)  (exact division) */
            binom = binom * (seq_t)(i - k) / (seq_t)k;

            if (k >= 3) {
                seq_t coeff = (seq_t)k * ((seq_t)k - 1) / 2; /* k*(k-1)/2 */
                seq_t prod  = coeff * binom * cache[i - k];
                sum += prod;
            }
        }
        cache[i] = term1 + sum;
    }
    max_cached = n;
}
#else
/* C build: use malloc/realloc to hold seq_t elements */
static seq_t *cache       = NULL;
static idx_t max_cached   = 2;       /* indices 0..2 already have value 0 */
static size_t cache_cap   = 3;       /* current allocation size */

/* Ensure that `cache` is allocated for indices up to `n`. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {    /* first use: allocate three entries */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* out of memory */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
        max_cached = 2;
    }

    if (n <= max_cached)
        return;

    /* Resize the array if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t) (n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
        /* Newly allocated elements are uninitialized; we will set them below. */
    }

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)0;     /* a(0)=a(1)=a(2)=0 */
            continue;
        }

        seq_t term1 = (seq_t)i * ((seq_t)i - 1) / 2; /* n*(n-1)/2 */
        seq_t sum   = 0;
        seq_t binom = (seq_t)1;                 /* C(n-1,0) */

        for (idx_t k = 1; k < i; ++k) {
            binom = binom * (seq_t)(i - k) / (seq_t)k;   /* exact division */

            if (k >= 3) {
                seq_t coeff = (seq_t)k * ((seq_t)k - 1) / 2; /* k*(k-1)/2 */
                seq_t prod  = coeff * binom * cache[i - k];
                sum += prod;
            }
        }
        cache[i] = term1 + sum;
    }
    max_cached = n;
}
#endif

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
