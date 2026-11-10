/*
 * A276624  The infinite trunk of ternary beanstalk with reversed subsections.
 * https://oeis.org/A276624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=2; for n>1, with t=2*A054861(a(n-1)): if t+1 is a
 *            power of 3 then a(n)=3^(1+A081604(t+1))-1, else a(n)=t
 * Range:     u8 n <= 49, u16 n <= 6250, u32 n <= 6250, u64 n <= 6250,
 *            u128 n <= 6250, big any n
 * Validated: standard, pass, 87 terms through n = 5905 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A276624_v3(seq_t m)
{
    /* exponent of 3 in m! (Legendre) */
    seq_t res = 0;
    seq_t t = m;
    while (t >= 3) {
        t /= 3;
        res += t;
    }
    return res;
}

static seq_t A276624_digits3(seq_t m)
{
    seq_t d = 1;
    seq_t t = m;
    while (t > 2) {
        t /= 3;
        d++;
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t cur = 2;
    for (idx_t i = 2; i <= n; i++) {
        seq_t t = 2 * A276624_v3(cur);
        seq_t x = t + 1;
        seq_t y = x;
        while (y % 3 == 0) y /= 3;
        if (y == 1) {
            seq_t d = A276624_digits3(x) + 1;
            seq_t p = 1;
            while (d > 0) {
                p *= 3;
                d--;
            }
            cur = p - 1;
        } else {
            cur = t;
        }
    }
    return cur;
}
