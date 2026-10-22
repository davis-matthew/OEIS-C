/*
 * A326476  A(n, k) = (m*k)! [x^k] MittagLefflerE(m, x)^n, for m = 2, n >= 0,
 *          k >= 0; square array read by descending antidiagonals.
 * https://oeis.org/A326476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n, k) = (2*k)! * [x^(2*k)] cosh(x)^n Sequence entry a(n) is
 *            read by descending antidiagonals of the square array where A(i,
 *            k) = (2*k)! * [x^(2*k)] cosh(x)^i. The values a(n) are returned
 *            for all n >= 0
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp); */

static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    /* locate diagonal s and position t inside it */
    idx_t s = 0;
    idx_t total = 0;
    while (1) {
        idx_t diag = s + 1;
        if (total + diag > n)
            break;
        total += diag;
        ++s;
    }
    idx_t t = n - total;      /* row index i */
    idx_t i = t;
    idx_t k = s - t;          /* column index k */

    if (k == 0)              /* A(i,0) = 1 for any i */
        return 1;

    if (i == 0)              /* A(0,k) = 0 for k > 0 */
        return 0;

    seq_t sum = 0;
    idx_t two_k = 2 * k;     /* exponent (non‑negative) */

    for (idx_t j = 0; j <= i; ++j) {
        seq_t bj = binom(i, j);
        long long diff_ll = (long long)i - 2LL * (long long)j;
        seq_t base_abs = (seq_t)(diff_ll < 0 ? -diff_ll : diff_ll);
        seq_t pw = seq_pow(base_abs, two_k);
        sum += bj * pw;
    }
    seq_t denom = seq_pow((seq_t)2, i);
    return sum / denom;
}
