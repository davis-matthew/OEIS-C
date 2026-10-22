/*
 * A307495  Expansion of Sum_{k>=0} k!*((1 - sqrt(1 - 4*x))/2)^k.
 * https://oeis.org/A307495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the main diagonal of the triangle with T(n,0)=n! and
 *            T(n,k)=T(n,k-1)+T(n-1,k) (k≥1). a(n)=T(n,n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compatibility macro for C/C++ malloc */
#ifdef __cplusplus
#  define MALLOC(sz) static_cast<seq_t*>(malloc(sz))
#  define REALLOC(ptr,sz) static_cast<seq_t*>(realloc(ptr,sz))
#else
#  define MALLOC(sz) ((seq_t*)malloc(sz))
#  define REALLOC(ptr,sz) ((seq_t*)realloc(ptr,sz))
#endif

seq_t a(idx_t n)
{
    /* Cached values of a(0) … a(cache_len‑1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Previous row of the triangle (size = i for i‑1) */
    static seq_t *prev = NULL;
    static idx_t prev_len = 0;

    /* Factorial accumulator: (i‑1)! before computing row i */
    static seq_t fac = (seq_t)1;

    /* Initialisation for n = 0 */
    if (cache_len == 0) {
        cache = MALLOC(sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;

        prev = MALLOC(sizeof(seq_t));
        prev[0] = (seq_t)1;           /* 0! = 1 */
        prev_len = 1;
        fac = (seq_t)1;               /* (0)! */
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Compute new rows up to and including row n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute i! in fac */
        fac = fac * (seq_t)i;

        /* Allocate current row of length i+1 */
        seq_t *cur = MALLOC((i + 1) * sizeof(seq_t));
        cur[0] = fac;                 /* cur[0] = i! */

        /* Fill the rest of the row using the recurrence */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prev_val = (k < prev_len) ? prev[k] : (seq_t)0;
            cur[k] = cur[k - 1] + prev_val;
        }

        /* a(i) is the last entry of the new row */
        seq_t val = cur[i];

        /* Extend the cache to store the new value */
        cache = REALLOC(cache, (i + 1) * sizeof(seq_t));
        cache[i] = val;
        cache_len = i + 1;

        /* Prepare for next iteration: prev = cur */
        free(prev);
        prev = cur;
        prev_len = i + 1;
    }

    return cache[n];
}
