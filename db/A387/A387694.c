/*
 * A387694  a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) *
 *          binomial(2*n-2*k+1,2*k+1).
 * https://oeis.org/A387694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387694: a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(2*n-2*k+1,2*k+1)
   Recurrence: a(0)=1, a(1)=6, a(2)=21, a(3)=76
   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) - a(n-4)   for n ≥ 4
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised sequence values */
static idx_t   len   = 0;        /* number of stored terms   */
static idx_t   cap   = 0;        /* allocated capacity      */

static void grow_cache(idx_t need)
{
    if (cap >= need) return;
    /* grow capacity, doubling each time */
    idx_t new_cap = cap == 0 ? 4 : cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* allocation failure */
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)76;

    /* ensure cache has enough space */
    grow_cache(n + 1);

    /* compute terms up to n if not already done */
    for (idx_t i = len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)6;
        else if (i == 2)
            cache[2] = (seq_t)21;
        else if (i == 3)
            cache[3] = (seq_t)76;
        else
        {
            /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) + 4*a(i-3) - a(i-4) */
            cache[i] = (cache[i-1] << 2) - (cache[i-2] << 1) + (cache[i-3] << 2) - cache[i-4];
        }
    }
    len = n + 1;
    return cache[n];
}
