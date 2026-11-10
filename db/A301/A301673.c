/*
 * A301673  Partial sums of A301672.
 * https://oeis.org/A301673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A301672(k), where A301672: b(0)=1, b(1)=4,
 *            b(2)=8, b(3)=13, b(4)=17, b(5)=20, b(6)=25; b(n) =
 *            b(n-1)-b(n-2)+2*b(n-3)-b(n-4)+b(n-5)-b(n-6) for n>=7
 * Range:     u8 n <= 10, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 955 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[7] = {1, 4, 8, 13, 17, 20, 25};
    seq_t sum = 0;
    idx_t lim = (n < 6) ? n : (idx_t)6;
    for (idx_t i = 0; i <= lim; ++i)
        sum += init[(size_t)i];
    if (n < 7)
        return sum;

    seq_t r[6];
    r[0] = init[6];
    r[1] = init[5];
    r[2] = init[4];
    r[3] = init[3];
    r[4] = init[2];
    r[5] = init[1];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t pos = r[0] + (seq_t)2 * r[2] + r[4];
        seq_t neg = r[1] + r[3] + r[5];
        seq_t v = pos - neg;
        r[5] = r[4];
        r[4] = r[3];
        r[3] = r[2];
        r[2] = r[1];
        r[1] = r[0];
        r[0] = v;
        sum += v;
    }
    return sum;
}
