/*
 * A373275  a(n) = Sum_{d|n} (-1)^(d-1) * 2^(n/d-1).
 * https://oeis.org/A373275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} (-1)^(d-1) * 2^(n/d-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t sumOdd = 0;
    seq_t sumEven = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            idx_t exp1 = n / d1 - 1;          /* n/d1 - 1 */
            seq_t term1 = pow_int((seq_t)2, exp1);
            if (d1 & 1)    /* odd */
                sumOdd += term1;
            else
                sumEven += term1;

            /* divisor d2 = n/i, skip if equal to d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t exp2 = n / d2 - 1;      /* i - 1 */
                seq_t term2 = pow_int((seq_t)2, exp2);
                if (d2 & 1)    /* odd */
                    sumOdd += term2;
                else
                    sumEven += term2;
            }
        }
    }

    /* The sum over odd divisors is always at least as large as the sum
       over even divisors, so subtraction is safe in an unsigned type. */
    return sumOdd - sumEven;
}
