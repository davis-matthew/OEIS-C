/*
 * A277935  Number of ways 2*n-1 people can vote on three candidates so that
 *          the Condorcet paradox arises.
 * https://oeis.org/A277935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n+3,5) = (n*(n-1)*(n+1)*(n+2)*(n+3))/60
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 190, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use unsigned __int128 to avoid overflow during the intermediate product. */
    unsigned __int128 prod = 1;
    idx_t base = n + 3;

    /* Multiply the five consecutive numbers (n+3)(n+2)(n+1)n(n-1) */
    for (idx_t i = 0; i < 5; ++i)
    {
        prod *= (unsigned __int128)(base - i);
    }

    /* Divide by 5! = 120, then multiply by 2. */
    prod /= 120;
    prod *= 2;

    return (seq_t)prod;
}
