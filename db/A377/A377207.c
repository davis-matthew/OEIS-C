/*
 * A377207  Number of n-digit numbers where every digit is either a 9 or
 *          adjacent to a 9.
 * https://oeis.org/A377207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=18, a(3)=99; Recurrence:
 *            a(n)=a(n-1)+9*a(n-2)+81*a(n-3) for n>=4 (OEIS A377207)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values (index 0 unused) */
    static seq_t *cache      = NULL; /* array of seq_t */
    static idx_t max_computed = 0;   /* largest n computed so far  */
    static size_t capacity    = 0;   /* allocated size of cache    */

    if (n == (idx_t)0)          /* sequence defined only for n>=1 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (!cache)
    {
        capacity = 4;                           /* minimal capacity: needs indices 1..3 */
        cache = alloc_seq_array(capacity);
        if (!cache) return (seq_t)0;            /* allocation failure */

        /* base terms (offset 1) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)99;
        max_computed = 3;
    }

    /* Grow the cache if required */
    if ((size_t)n >= capacity)
    {
        size_t new_cap = capacity;
        while ((size_t)n >= new_cap) new_cap <<= 1;

        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return (seq_t)0;        /* allocation failure */

        /* copy existing values */
        for (size_t i = 1; i <= capacity - 1; ++i)
            new_cache[i] = cache[i];

        free_seq_array(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        cache[i] = cache[i-1] +
                   ((seq_t)9) * cache[i-2] +
                   ((seq_t)81) * cache[i-3];
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
