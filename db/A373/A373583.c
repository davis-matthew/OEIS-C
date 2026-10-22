/*
 * A373583  Expansion of 1 / ( (1 - 16*x^4) * (1 - x/(1 - 16*x^4)^(1/4)) ).
 * https://oeis.org/A373583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 16^k * binomial(n/4,k) (with
 *            binomial defined for non–integral arguments)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = floor(n/4) */
    idx_t m = n / 4;
    seq_t sum  = 1;      /* k = 0 term */
    seq_t term = 1;      /* term_0 */

    for (idx_t k = 1; k <= m; ++k)
    {
        /* term_k = term_{k-1} * (n - 4*(k-1)) * 4 / k   */
        /* Compute the product in a wider type to avoid overflow   */
        unsigned __int128 prod = (unsigned __int128)term;
        prod *= (n - 4 * (k - 1));
        prod *= 4;
        prod /= k;
        term = (seq_t)prod;
        sum += term;
    }

    return sum;
}
