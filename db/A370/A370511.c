/*
 * A370511  Expansion of Sum_{k>=0} k! * ( x/(1-x^3) )^k.
 * https://oeis.org/A370511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)! * binomial(n-2*k-1, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 3;

    for (idx_t k = 0; k <= limit; ++k)
    {
        seq_t fact = factorial(n - 3 * k);

        seq_t bin;
        if (k == 0)
            bin = 1;                          /* binomial(...,0) = 1 */
        else
            bin = binomial(n - 2 * k - 1, k);

        sum += fact * bin;
    }

    return sum;
}
