/*
 * A294023  Sum of the differences of the larger and smaller parts in the
 *          partitions of n into two parts with the smaller part prime.
 * https://oeis.org/A294023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n - 2i)*A010051(i)
 * Range:     u8 n <= 50, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* floor(n/2) */
    idx_t half = n >> 1;
    if (half < 2)
        return 0;   /* no primes ≤ half */

    /* Allocate a boolean array for the sieve: 0 = prime, 1 = composite */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)half + 1, 1);
    if (!is_comp)
        return 0;   /* allocation failed – graceful fallback */

    /* Eratosthenes sieve up to 'half' */
    for (idx_t i = 2; i * i <= half; ++i)
    {
        if (!is_comp[i])
        {
            for (idx_t j = i * i; j <= half; j += i)
                is_comp[j] = 1;
        }
    }

    /* Sum (n - 2p) over all primes p ≤ half */
    seq_t sum = 0;
    for (idx_t p = 2; p <= half; ++p)
    {
        if (!is_comp[p])
            sum += (seq_t)(n - 2 * p);
    }

    free(is_comp);
    return sum;
}
