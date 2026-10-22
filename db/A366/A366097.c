/*
 * A366097  Expansion of (1/x) * Series_Reversion( x*(1+x-x^3)/(1+x)^4 ).
 * https://oeis.org/A366097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(3*n-k+3,n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;      /* a(0) = 1 */

    seq_t sum = 0;
    idx_t limit = n / 3;       /* floor(n/3) */

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t t1 = binomial(n + k, k);
        seq_t t2 = binomial(3 * n - k + 3, n - 3 * k);
        sum += t1 * t2;
    }

    return sum / ((seq_t)n + 1);
}
