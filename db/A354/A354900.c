/*
 * A354900  a(n) = n! * Sum_{d|n} d^d / (n/d)!.
 * https://oeis.org/A354900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} d^d / (n/d)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1, but guard against n==0 */

    /* n!  */
    seq_t fact_n = fact_upto(n);

    /* Accumulate the sum over divisors m of n, where m = n/d */
    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor m1 = i, corresponding d1 = n/m1 = n/i */
            idx_t m1 = i;
            idx_t d1 = n / m1;
            seq_t pow1   = big_pow((seq_t)d1, (seq_t)d1);
            seq_t ratio1 = fact_n / fact_upto(m1);
            sum += ratio1 * pow1;

            /* divisor m2 = n/i (if different) */
            idx_t m2 = n / i;
            if (m2 != m1) {
                idx_t d2 = n / m2;   /* which equals i */
                seq_t pow2   = big_pow((seq_t)d2, (seq_t)d2);
                seq_t ratio2 = fact_n / fact_upto(m2);
                sum += ratio2 * pow2;
            }
        }
    }

    return sum;
}
