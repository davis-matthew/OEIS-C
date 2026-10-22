/*
 * A354851  a(n) = (n-1)! * Sum_{d|n} d^(n/d).
 * https://oeis.org/A354851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n} d^(n/d) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the caller (offset 1) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t term1 = big_pow((seq_t)d1, (seq_t)(n / d1));
            sum += term1;

            if (d1 != d2) {
                seq_t term2 = big_pow((seq_t)d2, (seq_t)(n / d2));
                sum += term2;
            }
        }
    }
    seq_t fact = factorial(n - 1);
    return fact * sum;
}
