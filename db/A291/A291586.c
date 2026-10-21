/*
 * A291586  a(n) = ((2n-1)!!)^4 * Sum_{i=1..n} 1/(2*i-1)^4.
 * https://oeis.org/A291586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-1)!!)^4 * SUM_{i=1..n} 1/(2*i-1)^4 Recurrence:
 *            a(0)=0, a(1)=1,
 *            a(n+1)=((2*n-1)^4+(2*n+1)^4)*a(n)-(2*n-1)^8*a(n-1) for n>0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    size_t old_len = (size_t)cache_len;
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_len];
    if (!new_cache) return 0;          /* failure handling */
#else
    new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return 0;          /* failure handling */
#endif

    /* copy old values */
    for (size_t i = 0; i < old_len && i < new_len; ++i)
        new_cache[i] = cache[i];

    /* initialise new slots to zero */
    for (size_t i = old_len; i < new_len; ++i)
        new_cache[i] = 0;

    /* free old cache */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_len = new_len;

    /* initialise base cases if needed */
    if (new_len > 0)
        cache[0] = 0;
    if (new_len > 1)
        cache[1] = 1;

    /* compute missing values */
    for (idx_t k = (old_len > 1 ? old_len : 2); k <= n; ++k)
    {
        seq_t a_nm1 = cache[k - 1];
        seq_t a_nm2 = cache[k - 2];

        /* t = 2*k - 3, u = 2*k - 1 */
        seq_t t = (seq_t)(2 * k - 3);
        seq_t t2 = t * t;            /* t^2 */
        seq_t t4 = t2 * t2;          /* t^4 */
        seq_t u = t + 2;             /* 2*k - 1 */
        seq_t u2 = u * u;            /* u^2 */
        seq_t u4 = u2 * u2;          /* u^4 */

        seq_t coeff = t4 + u4;       /* (2k-3)^4 + (2k-1)^4 */
        seq_t t8 = t4 * t4;          /* (2k-3)^8 */

        seq_t val = coeff * a_nm1 - t8 * a_nm2;
        cache[k] = val;
    }

    return cache[n];
}
