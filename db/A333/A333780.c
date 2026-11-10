/*
 * A333780  a(n) = g(-n) - g(n), where g corresponds to the inverse of
 *          A333773.
 * https://oeis.org/A333780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = g(-n) - g(n), g = inverse of A333773. g(m) is found by
 *            writing m in balanced ternary (digits e_i in {-1,0,1}), then
 *            walking from the top with an alternating sign s=+1: each nonzero
 *            e_i gives coefficient c=e_i*s (digit 1 if c=1, digit 2 if c=-1),
 *            then s=-s. g(m) is the base-3 number with th...
 * Range:     u8 n <= 121, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Validated: standard, pass, 79 terms through n = 6354 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A333780_g(idx_t n, int neg)
{
    int e[72];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        idx_t r = t % 3;
        t /= 3;
        if (r == 2) {
            e[len++] = -1;
            t += 1;
        } else {
            e[len++] = (int)r;
        }
    }
    seq_t pw[72];
    if (len > 0) {
        pw[0] = (seq_t)1;
        for (size_t i = 1; i < len; ++i) pw[i] = pw[i - 1] * (seq_t)3;
    }
    seq_t res = (seq_t)0;
    int s = 1;
    for (size_t i = len; i > 0; --i) {
        int d = e[i - 1];
        if (neg) d = -d;
        if (d == 0) continue;
        int c = d * s;
        s = -s;
        if (c == 1) res += pw[i - 1];
        else res += pw[i - 1] * (seq_t)2;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t p = A333780_g(n, 0);
    seq_t q = A333780_g(n, 1);
    return q - p;
}
