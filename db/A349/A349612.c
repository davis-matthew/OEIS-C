/*
 * A349612  Dirichlet convolution of A342001 [{arithmetic derivative of
 *          n}/A003557(n)] with A325126 [Dirichlet inverse of rad(n)].
 * https://oeis.org/A349612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A342001(d) * A325126(n/d); A325126 is
 *            multiplicative with g(p^k) = -p*(1-p)^(k-1) for k>=1, g(1)=1,
 *            and A342001(d) = Sum_{p^v||d} v*rad(d)/p
 * Range:     i8 n <= 1772, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9934 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A349612_h(idx_t p, int k)
{
    if (k == 0) return (seq_t)1;
    seq_t r = -(seq_t)p;
    seq_t q = (seq_t)1 - (seq_t)p;
    for (int i = 1; i < k; i++) r *= q;
    return r;
}

seq_t a(idx_t n)
{
    idx_t pr[64];
    int ex[64];
    int e[64];
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int c = 0;
            while (t % p == 0) { t /= p; ++c; }
            pr[cnt] = p; ex[cnt] = c; ++cnt;
        }
    }
    if (t > 1) { pr[cnt] = t; ex[cnt] = 1; ++cnt; }

    for (int i = 0; i < cnt; i++) e[i] = 0;

    seq_t total = (seq_t)0;
    for (;;) {
        /* A342001(d) */
        seq_t rad = (seq_t)1;
        for (int i = 0; i < cnt; i++)
            if (e[i] > 0) rad *= (seq_t)pr[i];
        seq_t f = (seq_t)0;
        for (int i = 0; i < cnt; i++)
            if (e[i] > 0) f += (seq_t)e[i] * (rad / (seq_t)pr[i]);
        if (f != 0) {
            seq_t g = (seq_t)1;
            for (int i = 0; i < cnt; i++)
                g *= A349612_h(pr[i], ex[i] - e[i]);
            total += f * g;
        }
        /* next divisor */
        int i = 0;
        while (i < cnt) {
            if (e[i] < ex[i]) { e[i]++; break; }
            e[i] = 0; i++;
        }
        if (i == cnt) break;
    }
    return total;
}
