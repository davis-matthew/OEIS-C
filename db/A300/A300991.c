/*
 * A300991  O.g.f. A(x) satisfies: A(x) = x * (1 - 4*x*A'(x)) / (1 -
 *          5*x*A'(x)).
 * https://oeis.org/A300991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2 * a(n) = 5 * Σ_{k=1}^{n-1} k * a(k) * a(n-k) -
 *            4 * (n-1) * a(n-1) * This recurrence follows from the
 *            differential equation * A(x) = x (1-4xA'(x))/(1-5xA'(x)) for the
 *            OGF of A300991
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache = {0,1};
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_last = 0;   /* highest index that has been computed */
#endif

/* Make sure all terms up to and including n are computed. */
static void compute_up_to(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() == 0) {
        cache.push_back(0);
        cache.push_back(1);
    }
    while (cache.size() <= n) {
        idx_t m = (idx_t)cache.size();          /* next index to compute */
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = cache[m - k] * cache[k];
            prod *= (seq_t)k;
            sum += prod;
        }
        seq_t term1 = sum * (seq_t)5;
        seq_t term2 = cache[m - 1] * (seq_t)(4 * (m - 1));
        cache.push_back(term1 - term2);
    }
#else
    if (cache == NULL) {
        cache_cap = 2;                      /* indices 0 and 1 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache_last = 1;
    }
    /* Grow the raw array if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        /* new elements will be overwritten during the loop below */
    }
    idx_t start = cache_last + 1;
    if (start < 2) start = 2;               /* a(1) already known */
    for (idx_t m = start; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = cache[m - k] * cache[k];
            prod *= (seq_t)k;
            sum += prod;
        }
        seq_t term1 = sum * (seq_t)5;
        seq_t term2 = cache[m - 1] * (seq_t)(4 * (m - 1));
        cache[m] = term1 - term2;
        cache_last = m;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1; return 0 */
    compute_up_to(n);
#ifdef __cplusplus
    return cache[(size_t)n];
#else
    return cache[n];
#endif
}
