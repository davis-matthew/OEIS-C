/*
 * A283457  Row n=5 of A144048.
 * https://oeis.org/A283457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3*2^(n-1) + 3*2^(2*n-1) + 3^n + 5^n + 6^n Initial
 *            value: a(0) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3, pow6, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/power5.h"
#include "../utils/pow6.h"

seq_t a(idx_t n)
{
    /* Handle the special case a(0) = 7 */
    if (n == 0)
        return 7;

    /* Compute the various powers required by the formula */
    seq_t pow2_n1 = pow2(n - 1);         /* 2^(n-1)      */
    seq_t pow2_2n1 = pow2(2 * n - 1);    /* 2^(2n-1)     */
    seq_t pow3_n = pow3(n);              /* 3^n          */
    seq_t pow5_n = power5(n);            /* 5^n          */
    seq_t pow6_n = pow6(n);              /* 6^n          */

    /* Assemble the result */
    seq_t result = 1;
    result += 3 * pow2_n1;
    result += 3 * pow2_2n1;
    result += pow3_n;
    result += pow5_n;
    result += pow6_n;

    return result;
}
