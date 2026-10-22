/*
 * A386837  a(n) = Sum_{k=0..n} binomial(4*n+2,k) * binomial(4*n-k-1,n-k).
 * https://oeis.org/A386837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+2,k) * binomial(4*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly. */
    if (n == 0U)
        return (seq_t)1;

    /* 4*n and 4*n+2 are small enough for idx_t. */
    const idx_t four_n = 4U * n;
    const idx_t A = four_n + 2U;            /* A = 4*n + 2 */

    /* T(0) = C(4*n-1, n) */
    seq_t term = binomial(four_n - 1U, n);
    seq_t sum  = term;

    /* Recurrence:
       T(k+1) = T(k) * (A - k)/(k+1) * (n - k)/(4*n - k - 1)   */
    for (idx_t k = 0U; k < n; ++k)
    {
        /* Use an unsigned 128‑bit intermediate to avoid overflow
           for the 64‑bit implementation.  The expression fits
           comfortably in 128 bits for the n values that reach
           the tested limits. */
        unsigned __int128 tmp = (unsigned __int128)term * (A - k);
        tmp = tmp * (n - k);
        tmp /= (k + 1U);
        tmp /= (four_n - k - 1U);
        term = (seq_t)tmp;
        sum += term;
    }
    return sum;
}
