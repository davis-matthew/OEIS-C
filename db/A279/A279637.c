/*
 * A279637  Exponential transform of the fourth powers A000583.
 * https://oeis.org/A279637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   pow4_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = ∑_{k=0}^{n-1} binom(n-1,k) * a(k) * (n-k)^4
 *  with a(0)=1.
 *  See the note in the OEIS entry: the exponential transform
 *  of the fourth powers yields this recurrence.
 */

#include <stdlib.h>
#include "../utils/pow4_seq.h"

static seq_t *cache = NULL;        /* a[0..max_n]                           */
static size_t cache_cap = 0;       /* allocated capacity of cache           */
static size_t max_n = (size_t)-1;   /* highest n whose value is known */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;          /* a(0) = 1 */
        max_n = 0;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t cur = max_n + 1; cur <= (size_t)n; ++cur)
    {
        seq_t sum = 0;
        seq_t cur_binom = 1;              /* binom(cur-1,0) = 1 */

        for (size_t k = 0; k < cur; ++k)
        {
            seq_t pow = pow4_seq((seq_t)(cur - k));
            sum += cur_binom * cache[k] * pow;

            /* update binomial: binom(cur-1,k+1) = binom(cur-1,k)*(cur-1-k)/(k+1) */
            if (k + 1 < cur) /* avoid division by zero when k = cur-1 */
            {
                cur_binom = (cur_binom * (seq_t)(cur - 1 - k)) / (seq_t)(k + 1);
            }
        }
        cache[cur] = sum;
    }

    max_n = (size_t)n;
    return cache[n];
}
