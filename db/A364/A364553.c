/*
 * A364553  Number of edges in the n-Pell graph.
 * https://oeis.org/A364553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*A000129(n+1)/2 (A000129 is the Pell sequence: P(0)=0,
 *            P(1)=1, P(n)=2*P(n-1)+P(n-2))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
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

/* Static cache of Pell numbers */
static seq_t *pell = NULL;   /* array of Pell numbers, 0‐based index */
static size_t pell_cap = 0;  /* current allocated capacity of `pell`  */
static idx_t last_comp = 0;  /* largest index for which pell[i] is computed */

/* Initialise the cache with the first two Pell numbers if not already done. */
static void init_pell(void)
{
    if (pell == NULL) {
        /* Ensure space for indices 0 and 1 (two elements). */
        ensure_cache((idx_t)2, &pell, &pell_cap);
        pell[0] = 0;
        pell[1] = 1;
        last_comp = 1;
    }
}

/* Ensure that Pell numbers up to index `u` (inclusive) have been computed. */
static void ensure_pell(idx_t u)
{
    init_pell();          /* make sure the cache exists with P(0) and P(1) */
    if (u <= last_comp)  /* nothing to do */
        return;
    /* Make sure the array can hold indices up to `u`. */
    ensure_cache(u + 1, &pell, &pell_cap);
    /* Compute all remaining Pell numbers up to `u`. */
    for (idx_t i = last_comp + 1; i <= u; ++i) {
        /* Pell recurrence: P(i) = 2*P(i-1) + P(i-2) */
        pell[i] = ((seq_t)2 * pell[i-1]) + pell[i-2];
    }
    last_comp = u;
}

seq_t a(idx_t n)
{
    /* `a(0)` is defined to be 0. */
    if (n == 0U)
        return 0;

    /* We need P(n+1). */
    ensure_pell(n + 1);

    /* Compute a(n) = n * P(n+1) / 2 */
    seq_t prod = (seq_t)n * pell[n + 1];
    return prod / ((seq_t)2);
}
