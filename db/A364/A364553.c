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
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/ensure_cache.h"     /* ensures capacity for seq_t arrays */

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
