/*
 * A256880  n*n!/round(n/2).
 * https://oeis.org/A256880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n!/round(n/2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* factorial cache: fact_cache[i] = i! */
static idx_t fact_cache_size = 0;  /* allocated size (indices 0 .. size-1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* defined only for n >= 1 – offset handling */

    /* initialise cache if needed */
    if (fact_cache == NULL) {
        fact_cache_size = 1;
        fact_cache     = (seq_t*)malloc(sizeof(seq_t));
        if (!fact_cache) abort();
        fact_cache[0] = (seq_t)1;   /* 0! = 1 */
    }

    /* grow cache up to n */
    if (n >= fact_cache_size) {
        idx_t new_size = (n + 1 > fact_cache_size * 2)
                          ? (n + 1)
                          : fact_cache_size * 2;

        seq_t *tmp = (seq_t*)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();

        for (idx_t i = fact_cache_size; i < new_size; ++i)
            tmp[i] = tmp[i - 1] * (seq_t)i;   /* i! = (i-1)! * i */

        fact_cache = tmp;
        fact_cache_size = new_size;
    }

    /* compute a(n) = n * n! / round(n/2) */
    idx_t  denom_idx = (n + 1) / 2;            /* round(n/2) */
    seq_t  denom     = (seq_t)denom_idx;       /* divisor */
    seq_t  q         = fact_cache[n] / denom; /* n! / round(n/2)  */
    seq_t  res       = q * (seq_t)n;           /* final result   */

    return res;
}
