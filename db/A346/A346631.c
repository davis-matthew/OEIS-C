/*
 * A346631  Number of strongly asymmetric Boolean nested canalizing functions
 *          with n variables.
 * https://oeis.org/A346631
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ((1+sqrt(2))^(n-1) - (1-sqrt(2))^(n-1)) / sqrt(2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    = 2 * n! * P_{n-1} where P_k are Pell numbers (P_0=0, P_1=1, P_k=2P_{k-1}+P_{k-2}) */

#include <stdlib.h>

/* static caches for factorials and Pell numbers */
static seq_t *fact_cache = NULL;
static seq_t *pell_cache = NULL;
static idx_t  fact_last  = 0; /* largest n for which fact_cache[n] is valid (fact_cache[0] = 1) */
static idx_t  pell_last  = 1; /* largest k for which pell_cache[k] is valid (pell_cache[0]=0, pell_cache[1]=1) */
static idx_t  cache_size = 0; /* number of allocated array elements (indices 0..cache_size-1) */

/* compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* value for n < 2 is irrelevant */

    /* grow caches if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        fact_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        pell_cache = (seq_t *)realloc(pell_cache, new_size * sizeof(seq_t));
        cache_size = new_size;

        /* initialise base values if not yet initialised */
        if (fact_last == 0) fact_cache[0] = (seq_t)1;
        if (pell_last <= 1) {
            pell_cache[0] = (seq_t)0;
            pell_cache[1] = (seq_t)1;
        }
    }

    /* extend factorial cache up to n */
    for (idx_t i = fact_last + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_last = n;

    /* extend Pell cache up to n-1 */
    idx_t needed_pell = n - 1;
    if (needed_pell > pell_last) {
        /* start from max(pell_last+1, 2) because Pell recurrence uses indices >=2 */
        idx_t start = pell_last + 1;
        if (start < 2) start = 2;
        for (idx_t i = start; i <= needed_pell; ++i) {
            pell_cache[i] = (seq_t)2 * pell_cache[i - 1] + pell_cache[i - 2];
        }
        pell_last = needed_pell;
    }

    /* a(n) = 2 * n! * Pell(n-1) */
    seq_t result = fact_cache[n] * ((seq_t)2 * pell_cache[n - 1]);
    return result;
}
