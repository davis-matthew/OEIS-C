/*
 * A386397  a(n) = (3^(n+2) + 4^(2*n+1))/13.
 * https://oeis.org/A386397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+2) + 4^(2*n+1)) / 13 Recurrence: a(0)=1, a(1)=7,
 *            a(n)=19*a(n-1)-48*a(n-2) (n>1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed_up_to = 0;
    static size_t capacity = 0;

    /* initialisation: a(0)=1, a(1)=7 */
    if (cache == NULL)
    {
        capacity = 2;
        cache = seq_alloc(capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        computed_up_to = 1;
    }

    if (n <= computed_up_to)
        return cache[n];

    /* extend cache if necessary */
    if ((size_t)n >= capacity)
    {
        size_t new_cap = capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
        {
            new_cap = new_cap * 2;
            if (new_cap <= capacity) new_cap = capacity + 1;
        }
        cache = seq_realloc(cache, capacity, new_cap);
        capacity = new_cap;
    }

    /* compute new terms using recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        cache[i] = cache[i - 1] * (seq_t)19 - cache[i - 2] * (seq_t)48;
    }
    computed_up_to = n;
    return cache[n];
}
