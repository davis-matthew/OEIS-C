/*
 * A386552  Concatenate powers of 10.
 * https://oeis.org/A386552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(n+1) * a(n-1) + 10^n (a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A386552 – Concatenate powers of 10. */
#include <stdint.h>   /* Types idx_t, seq_t are defined by the build */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t val = (seq_t)1;           /* a(0)                      */
    seq_t ten = (seq_t)10;          /* constant 10                */

    seq_t pow10_i = ten;            /* 10^1  = 10                 */
    seq_t pow10_i_plus1 = pow10_i * ten; /* 10^2  = 100              */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* val = 10^(i+1) * val + 10^i   (for current i) */
        val = pow10_i_plus1 * val + pow10_i;

        /* Advance powers of ten for the next iteration */
        pow10_i = pow10_i_plus1;          /* now 10^(i+1)           */
        pow10_i_plus1 = pow10_i * ten;    /* now 10^(i+2)           */
    }

    return val;
}
