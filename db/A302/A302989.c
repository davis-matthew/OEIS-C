/*
 * A302989  a(n) = n^n + n*n + n.
 * https://oeis.org/A302989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n + n*n + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1; /* 0^0 is defined as 1 */
    }

    seq_t sn  = (seq_t)n;      /* cast index to sequence type */
    seq_t pow = big_pow(sn, sn); /* compute n^n */
    seq_t sq  = sn * sn;      /* compute n*n */

    return pow + sq + sn;     /* n^n + n*n + n */
}
