/*
 * A370218  Coefficient of x^n in the expansion of ( (1+x)^3 / (1-x^3)^3 )^n.
 * https://oeis.org/A370218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(3*n + k - 1, k) * binomial(3*n,
 *            n - 3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* local helper: compute binomial coefficient binomial(a, b) for 0 <= b <= a
   using multiplicative formula to keep intermediates small. */
static inline seq_t binom_local(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b == 0 || b == a) return (seq_t)1;
    /* use symmetry to reduce the number of iterations */
    if (b > a - b) b = a - b;
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= b; ++i)
    {
        result *= (seq_t)(a - b + i);
        result /= (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxk = n / (idx_t)3;
    idx_t three_n = (idx_t)3 * n;

    for (idx_t k = (idx_t)0; k <= maxk; ++k)
    {
        idx_t a1 = three_n + k - (idx_t)1;
        seq_t c1 = binom_local(a1, k);
        idx_t b2_n = n - (idx_t)3 * k;
        seq_t c2 = binom_local(three_n, b2_n);
        sum += c1 * c2;
    }

    return sum;
}
