/*
 * A251585  a(n) = 5^(n-3) * (n+1)^(n-5) * (16*n^3 + 87*n^2 + 172*n + 125).
 * https://oeis.org/A251585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n-3) * (n+1)^(n-5) * (16*n^3 + 87*n^2 + 172*n + 125)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute polynomial part */
    seq_t poly = (seq_t)16 * n * n * n
               + (seq_t)87 * n * n
               + (seq_t)172 * n
               + (seq_t)125;

    seq_t pow5_pos = 1, pow5_neg = 1;
    seq_t powN1_pos = 1, powN1_neg = 1;

    /* 5^(n-3) part: handle positive/negative exponents separately */
    if (n >= 3) {
        pow5_pos = big_pow((seq_t)5, (seq_t)(n - 3));
    } else {
        pow5_neg = big_pow((seq_t)5, (seq_t)(3 - n));
    }

    /* (n+1)^(n-5) part */
    if (n >= 5) {
        powN1_pos = big_pow((seq_t)(n + 1), (seq_t)(n - 5));
    } else {
        powN1_neg = big_pow((seq_t)(n + 1), (seq_t)(5 - n));
    }

    /* Multiply positive power factors */
    seq_t result = poly;
    if (pow5_pos != 1)   result *= pow5_pos;
    if (powN1_pos != 1)  result *= powN1_pos;

    /* Divide by negative power factors */
    if (pow5_neg != 1)   result /= pow5_neg;
    if (powN1_neg != 1)  result /= powN1_neg;

    return result;
}
