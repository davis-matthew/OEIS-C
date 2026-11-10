/*
 * A363984  a(n) = Sum_{k = 0..n}
 *          (-1)^(n+k)*binomial(n,k)*binomial(n+k,k)*A363983(k).
 * https://oeis.org/A363984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n}
 *            (-1)^(n+k)*binomial(n,k)*binomial(n+k,k)*A363983(k), with
 *            A363983(k) = Sum_{j = floor((k+1)/2)..k}
 *            (-1)^(k+j)*binomial(k,j)*binomial(k+j-1,j)*binomial(2*j,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A363984_binom(idx_t n, idx_t k)
{
    seq_t c = 1;
    idx_t i;
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) {
        c = c * (seq_t)(n - i);
        c = c / (seq_t)(i + 1);
    }
    return c;
}

static seq_t A363984_b(idx_t k)
{
    seq_t pos = 0, neg = 0;
    idx_t j;
    if (k == 0) return 1;
    for (j = (k + 1) / 2; j <= k; j++) {
        seq_t t = A363984_binom(k, j);
        t = t * A363984_binom(k + j - 1, j);
        t = t * A363984_binom(2 * j, k);
        if (((k + j) & 1) == 0) pos += t; else neg += t;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        seq_t t = A363984_binom(n, k);
        t = t * A363984_binom(n + k, k);
        t = t * A363984_b(k);
        if (((n + k) & 1) == 0) pos += t; else neg += t;
    }
    return pos - neg;
}
