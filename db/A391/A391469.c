/*
 * A391469  Number of words of length 3*n that can be formed with a
 *          three-letter alphabet when the number of letters of each type is
 *          == 1 (mod 3).
 * https://oeis.org/A391469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=6, a(2)=90, a(3)=2106 a(n) =
 *            27*(a(n-1)-a(n-2)+27*a(n-3)) for n >= 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms              */
static idx_t cache_len = 0;        /* number of elements currently stored  */

/* Ensure that cache contains a(0) .. a(n) inclusive */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                      /* already computed            */
        return;

    idx_t old_len = cache_len;              /* elements currently computed */
    size_t new_len = (size_t)(n + 1);        /* needed size (indices 0..n)   */
    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_cache)                         /* allocation failed : abort   */
        return;

    /* copy previous values */
    for (idx_t i = 0; i < old_len; ++i)
        new_cache[i] = cache[i];
    /* the rest are zero-initialised implicitly by malloc  */
    if (cache) free(cache);
    cache = new_cache;
    cache_len = new_len;

    /* initialise base cases */
    if (cache_len > 0) cache[0] = (seq_t)0;
    if (cache_len > 1) cache[1] = (seq_t)6;
    if (cache_len > 2) cache[2] = (seq_t)90;
    if (cache_len > 3) cache[3] = (seq_t)2106;

    /* compute any new terms using the recurrence */
    for (idx_t i = old_len; i < new_len; ++i) {
        if (i >= 4) {                      /* recurrence applies for i≥4  */
            seq_t term = (cache[i-1] - cache[i-2]) + (seq_t)27 * cache[i-3];
            cache[i] = (seq_t)27 * term;
        }
        /* for i < 4 the base values are already set       */
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
