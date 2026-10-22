/*
 * A374512  Number of ways to tile a 3 X n board with 2 X 2 and 3 X 3
 *          staircase tiles.
 * https://oeis.org/A374512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3) + 2*a(n-4) with initial values
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=4, a(4)=6
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort (sequence values beyond this point
           are not required for the problem constraints). */
        abort();
    }
    cache = tmp;
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else if (i == 3)
            cache[i] = (seq_t)4;
        else if (i == 4)
            cache[i] = (seq_t)6;
        else {
            seq_t v = ((seq_t)2) * cache[i-2] +
                      ((seq_t)4) * cache[i-3] +
                      ((seq_t)2) * cache[i-4];
            cache[i] = v;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
