/*
 * A317940  Numerators of sequence whose Dirichlet convolution with itself
 *          yields A046644.
 * https://oeis.org/A317940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = numerator of f(n), f multiplicative, f(1)=1, f(n) =
 *            (1/2)*(A046644(n) - Sum_{d|n, 1<d<n} f(d)*f(n/d)); computed per
 *            prime power with h(e)=2^e*f(p^e), h(0)=1, h(e) =
 *            (2^e*A046644(p^e) - Sum_{i=1}^{e-1} h(i)*h(e-i))/2,
 *            A046644(p^e)=2^(v_2(e!)+e)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 16383, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9522 (2026-11-08)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static idx_t A317940_v2fact(idx_t e)
{
    idx_t v = 0;
    while (e > 1) {
        e /= 2;
        v += e;
    }
    return v;
}

/* h(e) = 2^e * f(p^e), with all values integers */
static seq_t A317940_h(idx_t e, seq_t *h)
{
    seq_t val = pow2(2 * e + A317940_v2fact(e));
    for (idx_t i = 1; i < e; i++)
        val -= h[i] * h[e - i];
    return val / 2;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t exps[64];
    int cnt = 0;
    idx_t t = n;
    idx_t maxe = 0;
    idx_t omega_total = 0;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            exps[cnt++] = e;
            if (e > maxe) maxe = e;
            omega_total += e;
        }
    }
    if (t > 1) {
        exps[cnt++] = 1;
        if (maxe < 1) maxe = 1;
        omega_total += 1;
    }

    seq_t h[70];
    h[0] = 1;
    for (idx_t e = 1; e <= maxe; e++)
        h[e] = A317940_h(e, h);

    seq_t H = 1;
    for (int i = 0; i < cnt; i++)
        H *= h[exps[i]];

    if (H == 0)
        return (seq_t)0;

    idx_t k = 0;
    while (k < omega_total && (H & 1) == 0) {
        H >>= 1;
        k++;
    }
    return H;
}
