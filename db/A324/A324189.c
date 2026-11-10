/*
 * A324189  a(n) = A324122(A163511(n)).
 * https://oeis.org/A324189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A324122(A163511(n)), where A324122(m) = sigma(m) -
 *            gcd(m*d(m), sigma(m)); a(0)=0. A163511(n) is built already
 *            factored, so sigma and d come from its prime exponents
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 8205, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 93 terms through n = 9636 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static const uint32_t A324189_pr[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned ex[64];
    unsigned cnt = 0;
    idx_t prev = 0, bitpos = 1, tmp = n;
    while (tmp) {
        if (tmp & 1) {
            ex[cnt++] = (unsigned)(bitpos - prev - 1);
            prev = bitpos;
        }
        ++bitpos;
        tmp >>= 1;
    }
    /* extra factor p(cnt) */
    ex[cnt - 1] += 1;

    seq_t m = (seq_t)1, sg = (seq_t)1, d = (seq_t)1;
    for (unsigned i = 0; i < cnt; ++i) {
        if (ex[i] == 0) continue;
        seq_t p = (seq_t)A324189_pr[i];
        seq_t pw = (seq_t)1, s = (seq_t)1;
        for (unsigned e = 0; e < ex[i]; ++e) {
            pw *= p;
            s += pw;
        }
        m *= pw;
        sg *= s;
        d *= (seq_t)(ex[i] + 1);
    }
    seq_t g = gcd(m * d, sg);
    return sg - g;
}
