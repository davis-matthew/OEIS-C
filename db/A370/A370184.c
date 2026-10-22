/*
 * A370184  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x+x^3) )^n.
 * https://oeis.org/A370184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(3n-k,
 *            n-3k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n, k);
        idx_t m = 3 * n - k;
        idx_t r = n - 3 * k;
        seq_t t2 = binomial(m, r);
        sum += t1 * t2;
    }
    return sum;
}
