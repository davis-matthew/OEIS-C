/*
 * A391832  a(n) = (1/4) * Sum_{k=0..n} (k+2) * 2^k *
 *          binomial(2*k+2,2*n-2*k+1).
 * https://oeis.org/A391832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Σ_{k=0..n} (k+2) * 2^k * binomial(2k+2, 2n-2k+1)
 *            Recurrence (n ≥ 8): a(n) = 8*a(n-1) - 16*a(n-2) + 16*a(n-3) -
 *            72*a(n-4) + 32*a(n-5) - 64*a(n-6) + 64*a(n-7) - 16*a(n-8)
 *            Initial values: a(0)=1, a(1)=6, a(2)=30, a(3)=160, a(4)=824,
 *            a(5)=4112, a(6)=20240, a(7)=98432
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms   */
static idx_t cache_len = 0;          /* current number of cached terms    */
static const seq_t init_vals[8] = {1, 6, 30, 160, 824, 4112, 20240, 98432};

/* Ensure the cache contains at least indices 0..n.  Compute missing
   terms using the recurrence.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) /* allocation failure – abort */
        exit(1);
    cache = new_cache;

    for (idx_t i = cache_len; i < new_len; ++i)
    {
        if (i < 8)        /* initialise first eight terms */
            cache[i] = init_vals[i];
        else
        {
            /* compute using the linear recurrence in two sums to avoid
               unsigned underflow: pos_sum - neg_sum  */
            seq_t pos = 8 * cache[i - 1]
                      + 16 * cache[i - 3]
                      + 32 * cache[i - 5]
                      + 64 * cache[i - 7];

            seq_t neg = 16 * cache[i - 2]
                      + 72 * cache[i - 4]
                      + 64 * cache[i - 6]
                      + 16 * cache[i - 8];

            cache[i] = pos - neg;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Directly return the first eight terms without caching */
    if (n < 8)
        return init_vals[n];

    ensure_cache(n);
    return cache[n];
}
