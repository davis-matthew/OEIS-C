/*
 * A397381  a(n) = Sum_{k=0..n} binomial(n+2*k,3*k) * (binomial(n+3*k,k) +
 *          binomial(n+3*k,k-1)).
 * https://oeis.org/A397381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k,3*k) * * ( binomial(n+3*k,k)
 *            + binomial(n+3*k,k-1) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t n2k = n + 2 * k;
        idx_t n3k = n + 3 * k;
        idx_t t3k = 3 * k;          /* 3*k fits in idx_t            */
        seq_t term1 = binomial(n2k, t3k);

        seq_t bin1 = binomial(n3k, k);
        seq_t bin2 = 0;
        if (k > 0) bin2 = binomial(n3k, k - 1);

        seq_t inner = bin1 + bin2;
        result += term1 * inner;
    }
    return result;
}
