/*
 * A331257  Numerator of squared radius of inscribed circle of the n-th
 *          triangle with integer sides in the list given by A316841.
 *          Denominators are A331258.
 * https://oeis.org/A331257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (j+k-i)(i+k-j)(i+j-k)/(4(i+j+k)) in lowest
 *            terms, where (i,j,k) is the n-th triangle of A316841
 *            (i>=j>=k>=1, j+k>i, ordered by i, j, k); found by direct
 *            enumeration
 * Range:     u8 n <= 48, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {
        for (idx_t j = (i + 1) / 2; j <= i; ++j) {
            idx_t k = (i > j) ? (i - j + 1) : 1;
            for (; k <= j; ++k) {
                ++count;
                if (count == n) {
                    seq_t I = (seq_t)i, J = (seq_t)j, K = (seq_t)k;
                    seq_t num = (J + K - I) * (I + K - J) * (I + J - K);
                    seq_t den = (seq_t)4 * (I + J + K);
                    seq_t g = gcd(num, den);
                    return num / g;
                }
            }
        }
    }
}
