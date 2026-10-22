/*
 * A322661  Number of graphs with loops spanning n labeled vertices.
 * https://oeis.org/A322661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322661: Number of graphs with loops spanning n labeled vertices.
   Formula: a(n) = Σ_{k=0}^{n} (-1)^k * C(n,k) * 2^{(n-k+1)(n-k)/2} */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m   = n - k;                 /* remaining vertices */
        idx_t exp = (m + 1) * m / 2;        /* exponent for 2^...  */

        seq_t term = binomial(n, k) * pow2(exp);

        if ((k & 1) == 0)                 /* even k -> + */
            sum_pos += term;
        else                               /* odd k  -> - */
            sum_neg += term;
    }

    /* sum_pos >= sum_neg because the final result is positive */
    return sum_pos - sum_neg;
}
