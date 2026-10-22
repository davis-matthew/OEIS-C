/*
 * A391311  a(n) = Sum_{k=0..n} HurwitzZeta(k-n, k) - HurwitzZeta(k-n, n).
 * https://oeis.org/A391311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{n-1} k^{n-k} * (k^{k+1} - 1)/(k-1), a(0)=0,
 *            a(1)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t term;
        if (k == 1)
        {
            term = 2;
        }
        else
        {
            seq_t pow_nk   = big_pow((seq_t)k, (seq_t)(n - k));
            seq_t pow_kp1  = big_pow((seq_t)k, (seq_t)(k + 1));
            seq_t part     = (pow_kp1 - 1) / (k - 1);
            term = pow_nk * part;
        }
        sum += term;
    }
    return sum;
}
