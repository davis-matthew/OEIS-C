/*
 * A259463  From higher-order arithmetic progressions.
 * https://oeis.org/A259463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (recurrence form):
 *   a(0) = 5
 *   a(n) = ((n+5)*(n+4)*(3*n+8)*(n+3)^2)/(6*n*(3*n+5)) * a(n-1),
 *   for n ≥ 1
 *
 * The implementation reduces the fraction with the current value before
 * performing any intermediate multiplication, preventing overflow in
 * typical 64‑bit builds while remaining exact for arbitrary‑precision
 * builds.
 */

#include <stddef.h>   /* for size_t */

static seq_t gcd_seq(seq_t x, seq_t y) {
    while (y != (seq_t)0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;          /* a(0) */

    seq_t res = (seq_t)5;                 /* a(0) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;

        /* numerator and denominator factors of the ratio */
        seq_t numer[5] = { kseq + (seq_t)5,
                           kseq + (seq_t)4,
                           (seq_t)3 * kseq + (seq_t)8,
                           kseq + (seq_t)3,
                           kseq + (seq_t)3 };

        seq_t denom[3] = { (seq_t)6, kseq, (seq_t)3 * kseq + (seq_t)5 };

        /* cancel denominator factors with the current result (a(k-1)) */
        for (int i = 0; i < 3; ++i) {
            while (denom[i] != (seq_t)1) {
                seq_t g = gcd_seq(res, denom[i]);
                if (g == (seq_t)1) break;
                res /= g;
                denom[i] /= g;
            }
        }

        /* cancel remaining denominator factors with numerator factors */
        for (int i = 0; i < 3; ++i) {
            if (denom[i] == (seq_t)1) continue;
            for (int j = 0; j < 5; ++j) {
                if (numer[j] == (seq_t)1) continue;
                seq_t g = gcd_seq(numer[j], denom[i]);
                if (g == (seq_t)1) continue;
                numer[j] /= g;
                denom[i] /= g;
                if (denom[i] == (seq_t)1) break;
            }
        }

        /* multiply by numerator factors */
        for (int j = 0; j < 5; ++j) {
            res *= numer[j];
        }

        /* divide by any remaining denominator factors */
        for (int i = 0; i < 3; ++i) {
            if (denom[i] != (seq_t)1) {
                res /= denom[i];
            }
        }
    }

    return res;
}
