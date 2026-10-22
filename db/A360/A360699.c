/*
 * A360699  G.f.: Sum_{k>=0} (1 + k*x)^k * x^(2*k).
 * https://oeis.org/A360699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k, n-2*k) * k^(n-2*k)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 27, u64 n <= 46, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* k must satisfy n-2k >= 0 and n-2k <= k
       -> ceil(n/3) <= k <= floor(n/2) */
    idx_t start = (n + 2) / 3;   /* ceil(n/3)   */
    idx_t end   = n / 2;         /* floor(n/2)  */

    for (idx_t k = start; k <= end; ++k) {
        idx_t d = n - 2 * k;          /* n-2k  */
        if (d > k) continue;          /* binomial(k,d) will be zero otherwise */

        seq_t bin = binomial(k, d);
        if (bin == 0) continue;

        seq_t pow_val = (d == 0) ? (seq_t)1
                                 : big_pow((seq_t)k, (seq_t)d);

        sum += bin * pow_val;
    }
    return sum;
}
