/*
 * A303909  Expansion of 2*(1 - x)/(3 - theta_3(x)), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A303909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) - c(n-1) where c(0) = 1 c(n) = Σ_{k≥1, k^2≤n} c(n -
 *            k^2) (n ≥ 1) G.f.: (1 - x) / (1 - Σ_{k≥1} x^{k^2})
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the values of c(n).  It is safe for all
   positive seq_t sizes (including arbitrary‑precision).  The routine
   consequently works for any order of queries, recomputing only the
   missing values.  The helper 'ensure_cache' expands the storage array
   as required. */

#include <stddef.h>                     /* size_t */
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* Memoised array of the convolution sequence c(n) */
    static seq_t *cache   = NULL;  /* allocated by ensure_cache */
    static size_t cap     = 0;     /* number of entries currently allocated */
    static size_t computed = 0;    /* first index not yet computed (c[0..computed-1] known) */

    /* Ensure the internal stores are large enough for index 'n'  */
    if ((idx_t)cap <= n)
        ensure_cache(n, &cache, &cap);

    /* Initialise c(0) if nothing has been computed yet */
    if (computed == 0) {
        cache[0] = 1;          /* c(0) = 1 */
        computed = 1;
    }

    /* Compute c(i) for all i in [computed, n]  */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k * k <= (idx_t)i; ++k)
            sum += cache[i - k * k];
        cache[i] = sum;
    }
    computed = (size_t)n + 1;   /* now c[0..n] known */

    /* a(n) = c(n) - c(n-1)  (treat a(0) as c(0) )  */
    if (n == 0)
        return cache[0];
    else
        return cache[n] - cache[n - 1];
}
