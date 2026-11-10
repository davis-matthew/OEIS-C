/*
 * A303208  Number of total dominating sets in the n X n rook graph.
 * https://oeis.org/A303208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k*binomial(n,k)^2*k!*A287065(n-k), with
 *            A287065(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 50 (2026-11-08)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t A303208_pw(seq_t b, idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) r = r * b;
    return r;
}

static seq_t A303208_A287065(idx_t m)
{
    if (m == 0) return (seq_t)1;
    seq_t pos = A303208_pw(pow2(m) - (seq_t)1, m);
    seq_t neg = (seq_t)0;
    for (idx_t c = 1; c <= m; ++c) {
        seq_t base = pow2(m - c) - (seq_t)1;
        seq_t term = binomial(m, c) * A303208_pw(base, m);
        if (c & 1) pos += term; else neg += term;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    seq_t ff = (seq_t)1; /* n!/(n-k)! */
    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) ff = ff * (seq_t)(n - k + 1);
        seq_t term = binomial(n, k) * ff * A303208_A287065(n - k);
        if (k & 1) neg += term; else pos += term;
    }
    return pos - neg;
}
