/*
 * A309976  Vacation Dyck paths. Discrete analog for vacation M/M/1 queue
 *          embedded chain.
 * https://oeis.org/A309976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A309976: Vacation Dyck paths.
 * Recurrence (for n ≥ 6):
 *   n*a(n) = n*a(n-1) + 6*(n-2)*a(n-2) - 3*(n-4)*a(n-3)
 *           - 8*(n-3)*a(n-4) - 4*(n-3)*a(n-5)
 *
 * Initial values:
 *   a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 0, a(5) = 2
 *
 * The function uses a static cache so that calls may arrive in any order
 * and are computed in O(n) time with memoisation.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* Memoisation table */
static idx_t cached_size = 0; /* Number of terms already stored */

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (cached_size <= n)
    {
        size_t new_size = (size_t)n + (size_t)1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Initialise newly added entries */
        for (idx_t i = cached_size; i < new_size; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)0;
            else if (i == 3)
                cache[i] = (seq_t)1;
            else if (i == 4)
                cache[i] = (seq_t)0;
            else if (i == 5)
                cache[i] = (seq_t)2;
            else
            {
                /* Apply the recurrence */
                seq_t term = ((seq_t)i) * cache[i - 1];
                term += ((seq_t)6) * (((seq_t)(i - 2)) * cache[i - 2]);
                term -= ((seq_t)3) * (((seq_t)(i - 4)) * cache[i - 3]);
                term -= ((seq_t)8) * (((seq_t)(i - 3)) * cache[i - 4]);
                term -= ((seq_t)4) * (((seq_t)(i - 3)) * cache[i - 5]);
                cache[i] = term / ((seq_t)i);
            }
        }
        cached_size = new_size;
    }
    return cache[n];
}
