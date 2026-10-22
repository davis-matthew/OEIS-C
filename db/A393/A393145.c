/*
 * A393145  Minimal voucher cost for n distinct positive integer vouchers v_1,
 *          v_2, ..., v_n, where the cost is defined as v_1 + v_1*v_2 +
 *          v_2*v_3 + ... + v_(n-1)*v_n.
 * https://oeis.org/A393145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 8)/6 - (15 - 3*(-1)^n)/12, with a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1846, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                     /* a(1) = 1 */

    seq_t nn = (seq_t)n;                      /* cast index to seq_t */

    /* For even n: (n³ + 8*n - 6)/6
       For odd  n: (n³ + 8*n - 9)/6        */
    if (n & 1u)  /* odd */
    {
        return (nn * nn * nn + 8 * nn - 9) / 6;
    }
    else        /* even */
    {
        return (nn * nn * nn + 8 * nn - 6) / 6;
    }
}
