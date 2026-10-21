/*
 * A295432  a(n) = (12*n)!*(3*n)!*(2*n)!/((6*n)!*(6*n)!*(4*n)!*n!).
 * https://oeis.org/A295432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, for n≥1
 *            a(n)=a(n‑1)*6*(12*n-1)*(12*n-5)*(12*n-7)*(12*n-11) /
 *            (n*(2*n-1)*(6*n-1)*(6*n-5))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /*
     * For built‑in integer types (≤ 64 bit) we use a 128‑bit
     * intermediate to avoid overflow.  For arbitrary‑precision
     * seq_t the product fits in seq_t itself.
     */
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (sizeof(seq_t) <= 8)
        {
            /* 128‑bit computations ------------------------------------------------- */
            unsigned __int128 prod_num = 6u;
            prod_num *= (unsigned __int128)(12*i - 1);
            prod_num *= (unsigned __int128)(12*i - 5);
            prod_num *= (unsigned __int128)(12*i - 7);
            prod_num *= (unsigned __int128)(12*i - 11);

            unsigned __int128 prod_den = (unsigned __int128)i;
            prod_den *= (unsigned __int128)(2*i - 1);
            prod_den *= (unsigned __int128)(6*i - 1);
            prod_den *= (unsigned __int128)(6*i - 5);

            unsigned __int128 tmp = (unsigned __int128)val * prod_num;
            tmp /= prod_den;
            val = (seq_t)tmp;
        }
        else
        {
            /* arbitrary‑precision (seq_t) computations ----------------------------- */
            seq_t prod_num = (seq_t)6;
            prod_num = prod_num * (seq_t)(12*i - 1);
            prod_num = prod_num * (seq_t)(12*i - 5);
            prod_num = prod_num * (seq_t)(12*i - 7);
            prod_num = prod_num * (seq_t)(12*i - 11);

            seq_t prod_den = (seq_t)i;
            prod_den = prod_den * (seq_t)(2*i - 1);
            prod_den = prod_den * (seq_t)(6*i - 1);
            prod_den = prod_den * (seq_t)(6*i - 5);

            val = val * prod_num;
            val = val / prod_den;
        }
    }

    return val;
}
