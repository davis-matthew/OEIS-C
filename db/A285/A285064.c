/*
 * A285064  Row sums of Sheffer triangle S2[4,1] = A285061.
 * https://oeis.org/A285064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   a(n) = a(n-1) + Σ_{k=1..n} binomial(n-1, k-1) * 4^k * a(n-k)   */
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n) {
    /* Cache of previously computed values.  In C++ we use a vector
       so that constructors are called; in C we use a plain array.   */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {          /* initialize a(0) */
        cache.push_back((seq_t)1);
    }
    const size_t cached = cache.size();  /* number of terms stored */
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    size_t cached = 0;             /* number of terms stored */
    if (cache == NULL) {           /* first call – reserve space for a(0) */
        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) { /* allocation failure – terminate */ }
        cache[0] = (seq_t)1;
    }
    cached = (cache_cap > 0) ? 1 : 0;   /* we know cache[0] is valid */
#endif

    /* If the requested term is already in the cache, return it. */
#ifdef __cplusplus
    if ((size_t)n < cache.size()) return cache[n];
#else
    if (n < cached) return cache[n];
#endif

    /* Expand cache to size n+1, initializing new elements to 0. */
#ifdef __cplusplus
    if (cache.size() < (size_t)(n + 1)) cache.resize((size_t)(n + 1), (seq_t)0);
#else
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) { /* allocation failure – terminate */ }
        cache = newptr;
        /* initialize the newly allocated part to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }
#endif

    /* Compute terms from the first missing index up to n. */
#ifdef __cplusplus
    /* In C++ cache.size() == cached after resizing. */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        if (i == 0) { /* a(0) already set */ }
        else {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(i-1,0) */
            seq_t pow4 = (seq_t)4;           /* 4^1 */

            for (uint64_t k = 1; k <= i; ++k) {
                /* term = C(i-1,k-1) * 4^k * a(i-k) */
                sum += binom * pow4 * cache[(size_t)(i - k)];

                if (k < i) {
                    /* C(i-1,k) = C(i-1,k-1) * (i-k) / k */
                    binom = (binom * (seq_t)(i - k)) / (seq_t)k;
                    pow4 *= (seq_t)4;
                }
            }
            cache[(size_t)i] = cache[(size_t)(i - 1)] + sum;
        }
    }
#else
    size_t i_start = cached; /* may be 0 or 1 */
    if (i_start == 0) i_start = 1;            /* a(0) is already known */
    for (size_t i = i_start; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(i-1,0) */
        seq_t pow4 = (seq_t)4;           /* 4^1 */

        for (uint64_t k = 1; k <= i; ++k) {
            sum += binom * pow4 * cache[i - k];
            if (k < i) {
                binom = (binom * (seq_t)(i - k)) / (seq_t)k;
                pow4 *= (seq_t)4;
            }
        }
        cache[i] = cache[i - 1] + sum;
    }
#endif

    /* Return the requested term. */
#ifdef __cplusplus
    return cache[(size_t)n];
#else
    return cache[n];
#endif
}
