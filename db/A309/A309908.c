/*
 * A309908  a(n) is 2^n represented in bijective base-9 numeration.
 * https://oeis.org/A309908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = bijective base-9 value of 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t x      = pow2(n);        /* 2^n */
    seq_t res    = (seq_t)0;       /* result in decimal */
    seq_t P10    = (seq_t)1;       /* 10^k factor */
    const seq_t N9 = (seq_t)9;     /* base 9 constant */
    const seq_t N10 = (seq_t)10;   /* decimal 10 constant */

    /* convert x to bijective base‑9 digits and build the decimal value */
    while (x != (seq_t)0)
    {
        seq_t r = x % N9;          /* remainder 0 … 8 */
        seq_t q = x / N9;          /* quotient */

        if (r == (seq_t)0)        /* bijective digit 9 */
        {
            r = N9;
            x = q - (seq_t)1;
        }
        else                       /* normal digit 1…8 */
        {
            x = q;
        }

        res = res + r * P10;       /* append digit (least significant first) */
        P10 = P10 * N10;
    }

    return res;
}
