/*
 * A377147  a(n) = Sum_{k=0..floor(n/3)} binomial(k+2,2) *
 *          binomial(k,n-3*k)^2.
 * https://oeis.org/A377147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k+2,2) * binomial(k,
 *            n-3*k)^2
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t total = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin1 = binomial(k + 2, 2);          /* C(k+2, 2) */
        seq_t bin2 = binomial(k, n - 3 * k);     /* C(k, n-3k) */
        seq_t prod = bin2 * bin2;                /* square */
        prod = prod * bin1;                      /* multiply by bin1 */
        total = total + prod;                    /* accumulate */
    }
    return total;
}
