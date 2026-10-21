/*
 * A281859  Curious identities based on the Armstrong number 407 =
 *          A005188(13).
 * https://oeis.org/A281859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281859
   Formula: a(n) = (1 + 2^(1+n)*5^n + 2^(1+2*n)*25^n + 1000^n)/3   (offset 1) */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute powers: 2^n, 5^n, 25^n, 1000^n */
    seq_t pow2   = 1;
    seq_t pow5   = 1;
    seq_t pow25  = 1;
    seq_t pow1000 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;          /* 2^n */
        pow5 *= 5;          /* 5^n */
        pow25 *= 25;        /* 25^n = 5^(2n) */
        pow1000 *= 1000;    /* 1000^n */
    }

    /* Terms of the formula */
    seq_t term1 = pow2 * pow5 * 2;          /* 2^(1+n) * 5^n = 2 * 2^n * 5^n */
    seq_t term2 = pow2 * pow2 * pow25 * 2;  /* 2^(1+2n) * 25^n = 2 * (2^n)^2 * 25^n */

    /* Numerator */
    seq_t num = 1 + term1 + term2 + pow1000;

    /* Divide by 3 – the division is exact */
    return num / 3;
}
