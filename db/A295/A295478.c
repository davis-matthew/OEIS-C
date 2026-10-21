/*
 * A295478  a(n) = (24*n)!*(5*n)!*(2*n)!/((12*n)!*(10*n)!*(8*n)!*n!).
 * https://oeis.org/A295478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n)!*(5*n)!*(2*n)!/((12*n)!*(10*n)!*(8*n)!*n!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;
    seq_t den[31];

    for (idx_t cur = 1; cur <= n; ++cur) {
        idx_t base = cur - 1;
        size_t dcnt = 0;

        /* denominator factors */
        for (idx_t t = 1; t <= 12; ++t) den[dcnt++] = (seq_t)(12 * base + t);
        for (idx_t t = 1; t <= 10; ++t) den[dcnt++] = (seq_t)(10 * base + t);
        for (idx_t t = 1; t <= 8;  ++t) den[dcnt++] = (seq_t)(8  * base + t);
        den[dcnt++] = (seq_t)cur;          /* the factor n */

        /* numerator factors: 24, 5, and 2 */
        /* 24 factors */
        for (idx_t t = 1; t <= 24; ++t) {
            seq_t v = (seq_t)(24 * base + t);
            for (size_t j = 0; j < dcnt; ++j) {
                seq_t g = gcd(v, den[j]);
                if (g > 1) {
                    v /= g;
                    den[j] /= g;
                }
            }
            res *= v;
        }
        /* 5 factors */
        for (idx_t t = 1; t <= 5; ++t) {
            seq_t v = (seq_t)(5 * base + t);
            for (size_t j = 0; j < dcnt; ++j) {
                seq_t g = gcd(v, den[j]);
                if (g > 1) {
                    v /= g;
                    den[j] /= g;
                }
            }
            res *= v;
        }
        /* 2 factors */
        for (idx_t t = 1; t <= 2; ++t) {
            seq_t v = (seq_t)(2 * base + t);
            for (size_t j = 0; j < dcnt; ++j) {
                seq_t g = gcd(v, den[j]);
                if (g > 1) {
                    v /= g;
                    den[j] /= g;
                }
            }
            res *= v;
        }

        /* divide by remaining denominator factors */
        for (size_t j = 0; j < dcnt; ++j) {
            if (den[j] > 1) {
                res /= den[j];
            }
        }
    }
    return res;
}
