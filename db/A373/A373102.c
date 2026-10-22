/*
 * A373102  Determinants of Hankel matrices corresponding to digits of numbers
 *          with an odd number of digits.
 * https://oeis.org/A373102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) is the determinant of the Hankel matrix built from the
 *            decimal digits of the nth number that has an odd number of
 *            digits (0,1,2,…). The sequence is indexed from 1
 * Range:     i8 n <= 1400, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the n‑th term of OEIS sequence A373102. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined, but safe */

    /* Find the length of the n‑th odd‑digit integer. */
    idx_t idx = n - 1;        /* 0‑based offset among odd‑digit numbers */
    int len = 1;              /* current odd length */

    while (1) {
        idx_t count;
        if (len == 1) {
            count = 10;       /* numbers 0 … 9 */
        } else {
            /* 9 · 10^(len‑1) numbers of this length. */
            idx_t pow10 = 1;
            for (int i = 1; i < len; ++i)
                pow10 *= 10;
            count = 9 * pow10;
        }

        if (idx < count)
            break;            /* desired number is here */
        idx -= count;
        len += 2;             /* next odd length (3,5,…) */
    }

    /* Compute the numeric value of that number. */
    idx_t number;
    if (len == 1) {
        number = idx;
    } else {
        idx_t pow10 = 1;
        for (int i = 1; i < len; ++i)
            pow10 *= 10;      /* 10^(len‑1) */
        number = pow10 + idx;
    }

    /* Extract decimal digits most‑significant first. */
    unsigned char d[5] = {0, 0, 0, 0, 0};   /* supports up to 5 digits */
    if (len == 1) {
        d[0] = (unsigned char)(number);
    } else {
        idx_t tmp = number;
        for (int pos = len - 1; pos >= 0; --pos) {
            d[pos] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Compute the determinant of the Hankel matrix. */
    if (len == 1) {
        return (seq_t)d[0];
    } else if (len == 3) {
        seq_t a0 = (seq_t)d[0];
        seq_t a1 = (seq_t)d[1];
        seq_t a2 = (seq_t)d[2];
        return a0 * a2 - a1 * a1;
    } else { /* len == 5 */
        seq_t a0 = (seq_t)d[0];
        seq_t a1 = (seq_t)d[1];
        seq_t a2 = (seq_t)d[2];
        seq_t a3 = (seq_t)d[3];
        seq_t a4 = (seq_t)d[4];
        return a0 * a2 * a4
             - a0 * a3 * a3
             - a1 * a1 * a4
             + (seq_t)2 * a1 * a2 * a3
             - a2 * a2 * a2;
    }
}
