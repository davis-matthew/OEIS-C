/*
 * A285337  a(n) = denominator of A065642(n)/n.
 * https://oeis.org/A285337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A065642(n)), the denominator of A065642(n)/n;
 *            A065642 found by search over multiples of rad(n)
 * Range:     u8 n <= 16, u16 n <= 7078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9932 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A285337_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A285337_A065642(idx_t n)
{
    if (n == 1) return 1;

    idx_t ps[24];
    int cnt = 0;
    idx_t m = n;
    seq_t rad = 1;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            ps[cnt++] = p;
            rad *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        ps[cnt++] = m;
        rad *= (seq_t)m;
    }

    seq_t cand = ((seq_t)n / rad + 1) * rad;
    for (;; cand += rad) {
        seq_t t = cand;
        for (int i = 0; i < cnt; ++i) {
            seq_t p = (seq_t)ps[i];
            while (t % p == 0) t /= p;
        }
        if (t == 1) return cand;
    }
}

seq_t a(idx_t n)
{
    seq_t m = A285337_A065642(n);
    seq_t g = A285337_gcd((seq_t)n, m);
    return (seq_t)n / g;
}
