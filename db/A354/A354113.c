/*
 * A354113  The smallest number that contains all the digits of n in order but
 *          does not equal n.
 * https://oeis.org/A354113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + 9*10^(digits-1) if the first digit of n is 1, * a(n)
 *            = 10^digits + n otherwise. * * This implements the OEIS A354113:
 *            the smallest number that contains * all the digits of n in order
 *            but does not equal n
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* Special case: n = 0  → 10 */
    if (n == 0)
        return pow10(1);          /* 10 */

    /* Count the number of decimal digits of n (≥1) */
    int digits = 0;
    idx_t t = n;
    while (t > 0) {
        ++digits;
        t /= 10;
    }

    /* Find the most significant digit */
    idx_t msd = n;
    while (msd >= 10) msd /= 10;

    seq_t nseq = (seq_t)n;

    if (msd == 1) {
        /* Insert a 0 after the leading 1  →  n + 9 * 10^(digits-1) */
        seq_t powk = pow10(digits - 1);
        return nseq + (seq_t)9 * powk;
    } else {
        /* Insert a 1 before n  →  10^digits + n */
        seq_t powd = pow10(digits);
        return powd + nseq;
    }
}
