/*
 * A251151  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A251151
   Empirical recurrence (valid for all n ≥ 1):
   a(n) =  7*a(n-1) + 4*a(n-2) − 20*a(n-3) + 3*a(n-4) + 8*a(n-5) − 4*a(n-6)
   Initial values: a(1)=65, a(2)=470, a(3)=3374, a(4)=24233,
                   a(5)=173990, a(6)=1249276.
   The function computes a(n) for any n ≥ 1, memoizing results
   so subsequent calls are O(1).  It works for all supported
   integer/BigInteger types defined as seq_t. */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* n = 0 is not part of the sequence offset; return 0 for safety */
    if (n == 0) return 0;

    /* Static cache for memoization: cache[i] = a(i) for i ≥ 1 */
    static seq_t *cache = NULL;     /* dynamically allocated array */
    static idx_t cache_len = 1;     /* currently valid indices: 0 ... cache_len-1 */

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Resize the cache to accommodate index n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;

    /* Base values for a(1)…a(6) (index 0 unused) */
    const seq_t base[7] = {
        (seq_t)0,          /* a(0) – not used, but keeps indices aligned */
        (seq_t)65,
        (seq_t)470,
        (seq_t)3374,
        (seq_t)24233,
        (seq_t)173990,
        (seq_t)1249276
    };

    /* Initialise the cache up to index 6 if needed */
    for (idx_t i = cache_len; i <= 6 && i <= n; ++i)
        cache[i] = base[i];

    /* Compute further terms using the empirical recurrence */
    idx_t start = cache_len > 6 ? cache_len : 7; /* start from the first needed index */
    for (idx_t i = start; i <= n; ++i) {
        seq_t term1 = 7 * cache[i - 1];
        seq_t term2 = 4 * cache[i - 2];
        seq_t term3 = 20 * cache[i - 3];
        seq_t term4 = 3 * cache[i - 4];
        seq_t term5 = 8 * cache[i - 5];
        seq_t term6 = 4 * cache[i - 6];

        /* Compute the sum of the positive parts first */
        seq_t sum_pos = term1 + term2 + term4 + term5;
        /* Then subtract the negative parts (safe because the result is non‑negative) */
        seq_t value = sum_pos - term3;
        value -= term6;

        cache[i] = value;
    }

    cache_len = n + 1;  /* update the cache length */
    return cache[n];
}
