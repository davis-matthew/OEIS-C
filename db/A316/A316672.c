/*
 * A316672  Numbers k for which 120*k + 169 is a square.
 * https://oeis.org/A316672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4*k+1) = (3*k+2)*(5*k-1)/2; a(4*k+2) = k*(15*k+13)/2; a(4*k+3)
 *            = (k+1)*(15*k+2)/2; a(4*k+4) = (3*k+1)*(5*k+6)/2
 * Range:     i8 n <= 16, i16 n <= 264, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res;
    idx_t k = (n - 1) / 4;
    switch (n % 4) {
    case 1: {
        seq_t t1 = ((seq_t)3 * (seq_t)k) + 2;
        seq_t t2 = ((seq_t)5 * (seq_t)k) - 1;
        res = t1 * t2 / 2;
        break;
    }
    case 2: {
        seq_t t1 = (seq_t)k;
        seq_t t2 = ((seq_t)15 * (seq_t)k) + 13;
        res = t1 * t2 / 2;
        break;
    }
    case 3: {
        seq_t t1 = ((seq_t)(k + 1));
        seq_t t2 = ((seq_t)15 * (seq_t)k) + 2;
        res = t1 * t2 / 2;
        break;
    }
    case 0: {
        seq_t t1 = ((seq_t)3 * (seq_t)k) + 1;
        seq_t t2 = ((seq_t)5 * (seq_t)k) + 6;
        res = t1 * t2 / 2;
        break;
    }
    default:
        res = 0;
        break;
    }
    return res;
}
