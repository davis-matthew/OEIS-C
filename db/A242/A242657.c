/*
 * A242657  Mahonian numbers T(n,7) (cf. A008302).
 * https://oeis.org/A242657
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,7), the Mahonian number counting permutations of * n
 *            elements with exactly 7 inversions. Recurrence: * T(0,0) = 1,
 *            T(0,k) = 0 for k>0 * T(n,k) = Σ_{i=0}^{min(k,n-1)} T(n-1,k-i)
 *            for n>0 * The function returns a(n) for all n >= 5 (offset 5).
 *            It memoizes * previously computed values to ha...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Single function required by the build */
seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache      = NULL;      /* cache[n] = T(n,7) */
    static size_t  cache_len = 0;         /* allocated length of cache */
    static seq_t  prev_row[8] = {0,0,0,0,0,0,0,0};  /* T(m-1,k) */
    static seq_t  cur_row[8]  = {0,0,0,0,0,0,0,0};  /* T(m,k)   */
    static idx_t  last_n     = 0;         /* highest n already computed */

    /* Initialise the first row (n = 0) once */
    if (cache == NULL) {
        const size_t init_len = 1;                  /* will grow as needed */
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (cache == NULL)                          /* memory allocation failed */
            return 0;
        cache[0] = 1;                               /* T(0,0) = 1 */
        cache_len = init_len;
        prev_row[0] = 1; /* T(0,0) */
        last_n = 0;
    }

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* cannot allocate more memory */
        cache = tmp;
        /* Initialise new entries to 0 (they will be overwritten) */
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        /* cur_row[k] = Σ_{i=0}^{min(k,m-1)} prev_row[k-i] */
        for (idx_t k = 0; k < 8; ++k) {
            seq_t sum = 0;
            for (idx_t i = 0; i <= k && i < m; ++i)
                sum += prev_row[k - i];
            cur_row[k] = sum;
        }
        /* Store the required Mahonian number T(m,7) */
        cache[m] = cur_row[7];
        /* Prepare for next iteration: prev_row = cur_row */
        for (idx_t k = 0; k < 8; ++k)
            prev_row[k] = cur_row[k];
    }
    last_n = n;

    return cache[n];
}
