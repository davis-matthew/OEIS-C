/*
 * A329924  Beatty sequence for (8+sqrt(34))/5.
 * https://oeis.org/A329924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8*n + floor(n*sqrt(34)))/5)
 * Range:     u8 none, u16 n <= 10, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for the unsigned integer type seq_t */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t best = 0;
    while (low <= high)
    {
        seq_t mid     = (low + high) >> 1;
        seq_t midsq   = mid * mid;
        if (midsq <= x)
        {
            best = mid;
            low  = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence starts at n = 1 */
    seq_t nn  = (seq_t)n;
    seq_t tmp = nn * nn;             /* n^2 */
    tmp       = tmp * (seq_t)34;      /* 34 * n^2 */
    seq_t sqrt_term = floor_sqrt_seq(tmp);  /* floor(n * sqrt(34)) */
    return (nn * (seq_t)8 + sqrt_term) / (seq_t)5;
}
