/*
 * A279028  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 129", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=101, a(3)=0; For n≥4,
 *            a(n)=101·a(n−2)−100·a(n−4); All odd indices produce 0. The
 *            recurrence can be computed in O(1) time per value and is
 *            memoised in a static array
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;  /* memoisation array */
static idx_t cache_size = 0;     /* current number of elements stored (size_t beware) */

/* Extend the cache so that it contains values up to n inclusive. */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp)              /* allocation failed – keep old cache */
        return;

    seq_cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
            seq_cache[i] = (seq_t)1;
        else if (i == 1)
            seq_cache[i] = (seq_t)0;
        else if (i == 2)
            seq_cache[i] = (seq_t)101;
        else if (i == 3)
            seq_cache[i] = (seq_t)0;
        else if ((i & 1) == 0)           /* even indices >= 4 */
        {
            seq_t a_im2 = seq_cache[i - 2];
            seq_t a_im4 = seq_cache[i - 4];
            seq_cache[i] = (seq_t)(101 * a_im2 - 100 * a_im4);
        }
        else                               /* odd indices */
            seq_cache[i] = (seq_t)0;
    }
    cache_size = new_len;
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return seq_cache[n];
}
