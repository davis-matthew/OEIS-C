/*
 * A342828  a(n) = Sum_{d|n} (-1)^(n/d+1) * d^(n-d).
 * https://oeis.org/A342828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d|n} (-1)^(n/d+1) * d^(n-d) (OEIS A342828)
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 19, i64 n <= 29, i128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"          /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0, return 0 */

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* First divisor d1 = i */
        idx_t d1 = i;
        idx_t nd1 = n / d1;                     /* n/d1 */
        seq_t sign1 = (( (nd1 + 1) & 1 ) ? -1 : 1);
        seq_t pow1  = big_pow((seq_t)d1, (seq_t)(n - d1));
        sum += sign1 * pow1;

        /* Second divisor d2 = n/i, skip if the same as d1 */
        idx_t d2 = n / i;
        if (d2 == d1) continue;
        idx_t nd2 = n / d2;                     /* n/d2 == i */
        seq_t sign2 = (( (nd2 + 1) & 1 ) ? -1 : 1);
        seq_t pow2  = big_pow((seq_t)d2, (seq_t)(n - d2));
        sum += sign2 * pow2;
    }
    return sum;
}
