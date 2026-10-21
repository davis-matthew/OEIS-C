/*
 * A262139  The first of eight consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of nine
 *          consecutive positive integers.
 * https://oeis.org/A262139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 3 Initial values:
 *            a(1) = 22, a(2) = 145, a(3) = 862
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised terms (1‑based index, stored at cache[0] etc.) */
    static seq_t cache[10001];          /* enough for n up to ~10000 */
    static idx_t cached = 0;            /* number of terms currently cached */

    /* initialise first three terms on first use */
    if (cached == 0)
    {
        /* cache[0] = a(1), cache[1] = a(2), cache[2] = a(3) */
        cache[0] = (seq_t)22;
        cache[1] = (seq_t)145;
        cache[2] = (seq_t)862;
        cached = 3;
    }

    /* if requested term is already cached, return it immediately */
    if (n <= cached)
        return cache[n - 1];

    /* extend the cache up to n using the recurrence */
    for (idx_t i = cached; i < n; ++i)   /* i is 0‑based, cache[i] = a(i+1) */
    {
        cache[i] = ((seq_t)7 * cache[i - 1])          /* 7*a(n-1)   */
                 - ((seq_t)7 * cache[i - 2])          /* -7*a(n-2)  */
                 +  cache[i - 3];                    /* +a(n-3)    */
    }

    cached = n;
    return cache[n - 1];
}
