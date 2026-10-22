/*
 * A335153  a(1)=0; thereafter a(n) = (n-1)*sigma(n)-n*sigma(n-1) where sigma
 *          is the sum-of-divisors function A000203.
 * https://oeis.org/A335153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)*sigma(n) - n*sigma(n-1) (sigma = sum of divisors)
 * Range:     i8 n <= 11, i16 n <= 143, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute sigma(n) : sum of positive divisors of n
   uses idx_t for accumulation to avoid overflow in seq_t */
static seq_t sigma_uint64(idx_t n)
{
    idx_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += i;
            idx_t j = n / i;
            if (j != i) sum += j;
        }
    }
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t sn     = (seq_t)n;
    seq_t sigma_n   = sigma_uint64(n);
    seq_t sigma_nm1 = sigma_uint64(n - 1);
    return (sn - 1) * sigma_n - sn * sigma_nm1;
}
