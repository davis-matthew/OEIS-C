/*
 * A372947  Decimal expansion of Sum_{k >= 0} (10^5*A196563(k) -
 *          A196564(k)/10^5)/10^k.
 * https://oeis.org/A372947
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(S*10^(n-11)) mod 10, with S = Sum_{k>=0}
 *            (10^5*A196563(k) - A196564(k)/10^5)/10^k; computed exactly in
 *            fixed point with carries
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9672 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A196563_cnt(uint64_t n)
{
    if (n == 0U) return 1U;
    uint64_t c = 0;
    while (n != 0U) {
        if (((n % 10U) & 1U) == 0U) c++;
        n /= 10U;
    }
    return c;
}

static uint64_t A196564_cnt(uint64_t n)
{
    uint64_t c = 0;
    while (n != 0U) {
        if ((n % 10U) & 1U) c++;
        n /= 10U;
    }
    return c;
}

seq_t a(idx_t n)
{
    const int G = 100;
    int64_t c[128];
    int i;
    for (i = 0; i < 128; i++) c[i] = 0;

    /* X = 10^5 * S = sum d(k) 10^-k, d(k) = 10^10*E(k) - O(k).
       Fixed point with scale 10^M, M = n-16+G; digit of interest at position G. */
    int64_t M = (int64_t)n - 16 + G;
    int64_t k0 = (int64_t)n - 16;
    if (k0 < 0) k0 = 0;
    int64_t k;
    for (k = k0; k <= M; k++) {
        int64_t p = M - k;
        int64_t d = (int64_t)A196563_cnt((uint64_t)k) * 10000000000LL
                  - (int64_t)A196564_cnt((uint64_t)k);
        c[p] += d;
    }

    int64_t carry = 0;
    int dg = 0;
    for (i = 0; i <= G; i++) {
        int64_t t = c[i] + carry;
        int64_t m = t % 10;
        if (m < 0) m += 10;
        dg = (int)m;
        carry = (t - m) / 10;
    }
    return (seq_t)dg;
}
