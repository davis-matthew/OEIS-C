/*
 * A361528  a(n) = (2+n)*(2*a(n-1) - (n-2)*a(n-2)) with a(0)=a(1)=1.
 * https://oeis.org/A361528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*(2*a(n-1) - (n-2)*a(n-2)); a(0)=a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* Handle the initial values directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Allocate or grow the cache if necessary */
    if (!cache || n > max_n)
    {
        size_t new_cap = (size_t)(n + 1);
        if (!cache)
        {
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
            if (!cache)
                exit(1);
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            max_n   = 1;
        }
        else if (new_cap > max_n + 1)
        {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache)
                exit(1);
        }

        /* Compute values from the first uncached index up to n */
        for (idx_t k = max_n + 1; k <= n; ++k)
        {
            seq_t two_a_prev = cache[k - 1] * (seq_t)2;
            seq_t sub        = cache[k - 2] * (seq_t)(k - 2);
            seq_t diff       = two_a_prev - sub;
            cache[k] = ((seq_t)(k + 2)) * diff;
        }
        max_n = n;
    }

    return cache[n];
}
