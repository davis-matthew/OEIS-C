/*
 * A316846  Column 1 of table A316842.
 * https://oeis.org/A316846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k appears A123323(k) times; A123323(k) = Sum_{d|k}
 *            mu(d)*floor((k/d+1)^2/4). a(n) = k where the cumulative sum of
 *            A123323(1..k) first reaches n (search)
 * Range:     u8 n <= 267, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 100 terms through n = 2732 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A316846_mu(idx_t x)
{
    int mu = 1;
    idx_t t = x;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return 0;
            mu = -mu;
        }
    }
    if (t > 1) mu = -mu;
    return mu;
}

static seq_t A316846_cnt(idx_t k)
{
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= k; ++d) {
        if (k % d != 0) continue;
        int mu = A316846_mu(d);
        if (mu == 0) continue;
        seq_t mm = (seq_t)(k / d) + 1;
        seq_t val = (mm * mm) / 4;
        if (mu > 0) pos += val; else neg += val;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t total = 0;
    idx_t k = 0;
    while (total < (seq_t)n) {
        ++k;
        total += A316846_cnt(k);
    }
    return (seq_t)k;
}
