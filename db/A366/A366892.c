/*
 * A366892  a(n) = A336652(A163511(n)).
 * https://oeis.org/A366892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336652(A163511(n)); with prime p(i+1) having exponent
 *            (zeros before i-th set bit of n) + [i = popcount(n)-1], a(n) =
 *            prod over i>=1 of (p + p^2 + ... + p^e) where p = p(i+1), e > 0
 * Range:     u8 n <= 32, u16 n <= 258, u32 n <= 8206, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 92 terms through n = 8959 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A366892_term(uint64_t p, idx_t e)
{
    seq_t term = 0;
    seq_t power = 1;
    for (idx_t k = 0; k < e; ++k) {
        power *= (seq_t)p;
        term += power;
    }
    return term;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* first 64 primes */
    uint64_t pr[64];
    int np = 0;
    for (uint64_t c = 2; np < 64; ++c) {
        int ok = 1;
        for (int j = 0; j < np && pr[j] * pr[j] <= c; ++j)
            if (c % pr[j] == 0) { ok = 0; break; }
        if (ok) pr[np++] = c;
    }

    int r = 0;
    for (idx_t t = n; t; t >>= 1) r += (int)(t & 1);

    seq_t result = (seq_t)1;
    int prev = -1;
    int i = 0;
    for (int b = 0; b < 64; ++b) {
        if (((n >> b) & 1) == 0) continue;
        idx_t e = (idx_t)(b - prev - 1);
        if (i == r - 1) e += 1;
        prev = b;
        if (i >= 1 && e > 0)
            result *= A366892_term(pr[i], e);
        ++i;
    }
    return result;
}
