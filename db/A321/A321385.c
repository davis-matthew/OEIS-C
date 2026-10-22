/*
 * A321385  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^d.
 * https://oeis.org/A321385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321385
    Formula: a(n) = Sum_{d|n} (-1)^(n/d + 1) * d^d
    (offset 1: a(1) is the first term)
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Returns the n-th term of the sequence.
   n must be >= 1, behaviour for n==0 is undefined but returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t sum_pos = 0;   /* contributes positively */
    seq_t sum_neg = 0;   /* contributes negatively */

    /* iterate over divisors by scanning up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) {
            continue;
        }

        /* divisor d1 = i */
        idx_t d1 = i;
        seq_t term1 = big_pow((seq_t)d1, (seq_t)d1);
        if ((n / d1) & 1ULL) {          /* n/d1 is odd → sign = +1 */
            sum_pos += term1;
        } else {                        /* n/d1 is even → sign = -1 */
            sum_neg += term1;
        }

        /* divisor d2 = n / i, if distinct from d1 */
        idx_t d2 = n / i;
        if (d2 != d1) {
            seq_t term2 = big_pow((seq_t)d2, (seq_t)d2);
            if ((n / d2) & 1ULL) {      /* n/d2 is odd → sign = +1 */
                sum_pos += term2;
            } else {                    /* n/d2 is even → sign = -1 */
                sum_neg += term2;
            }
        }
    }

    return sum_pos - sum_neg;
}
