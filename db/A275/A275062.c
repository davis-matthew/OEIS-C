/*
 * A275062  Number A(n,k) of permutations p of [n] such that p(i)-i is a
 *          multiple of k for all i in [n]; square array A(n,k), n>=0, k>=0,
 *          read by antidiagonals.
 * https://oeis.org/A275062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = ∏_{i=0}^{k-1} ⌊(n+i)/k⌋!
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 117, u64 n <= 273,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* factorial cache: fact_cache[i] = i!  */
static idx_t fact_cache_cap = 0;   /* number of cached factorials */

/* Ensure that factorials up to value n are in fact_cache.
   fact_cache[i] holds i! for 0 <= i < fact_cache_cap. */
static void ensure_fact(idx_t n)
{
    if (n + 1 <= fact_cache_cap)
        return;

    seq_t *new_cache = (seq_t*)realloc(fact_cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        exit(1);  /* out of memory */

    if (fact_cache_cap == 0) {
        new_cache[0] = 1;            /* 0! = 1 */
        fact_cache_cap = 1;
    }

    for (idx_t i = fact_cache_cap; i <= n; ++i) {
        new_cache[i] = new_cache[i - 1] * (seq_t)i;
    }

    fact_cache = new_cache;
    fact_cache_cap = n + 1;
}

/* Return the nth term (offset 0) of OEIS A275062. */
seq_t a(idx_t index)
{
    /* Find the diagonal d such that d*(d+1)/2 <= index < (d+1)*(d+2)/2. */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= index)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= index)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;

    idx_t start = d * (d + 1) / 2;          /* first index on diagonal d */
    idx_t offset = index - start;           /* position within the diagonal */

    idx_t n = offset;                       /* row index in the array */
    idx_t k = d - offset;                   /* column index (k >= 0) */

    if (k == 0)
        return 1;                           /* empty product */

    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        idx_t m = (n + i) / k;              /* floor((n+i)/k) */
        ensure_fact(m);
        result *= fact_cache[m];
    }
    return result;
}
