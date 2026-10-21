/*
 * A248332  Square array read by antidiagonals downwards: super Patalan
 *          numbers of order 8.
 * https://oeis.org/A248332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_gcd_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248332: Super Patalan numbers of order 8 (square array read by antidiagonals)
 *
 * Recurrence from OEIS:
 *   T(0,0) = 1
 *   T(n,k) = T(n-1,k) * (64*n-8) / (n+k)   for n>0
 *   T(n,k) = T(n,k-1) * (64*k-56) / (n+k)  for k>0
 *   a(n)   = T(i,j)  where i+j = d and n = d*(d+1)/2 + i
 *
 * The function below evaluates a(n) for any non‑negative n, using exact arithmetic.
 * It contains an internal implementation of gcd for arbitrary‑precision integers
 * and computes the required entry by applying the recurrence in the row and
 * column directions, cancelling factors with the Euclidean algorithm to
 * avoid overflow in intermediate terms.
 */

#include <stdint.h>  /* for uint64_t */
#include "../utils/seq_gcd_generic.h"

/* Multiply v by (num / den) exactly, cancelling factors with GCDs. */
static seq_t mul_div(seq_t v, idx_t num, idx_t den)
{
    seq_t m = (seq_t)num;
    seq_t d = (seq_t)den;

    /* cancel common factor between m and d */
    seq_t g = seq_gcd_generic(m, d);
    if (g > 1) {
        m /= g;
        d /= g;
    }

    /* cancel remaining factor of d with v */
    g = seq_gcd_generic(v, d);
    if (g > 1) {
        v /= g;
        d /= g;
    }

    /* After exact division the remaining denominator should be 1 */
    return v * m;
}

/* Find the diagonal d, row offset i, and column j for a given index n.
 * The diagonal d satisfies d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
static void diag_of(idx_t n, idx_t *d_out, idx_t *i_out, idx_t *j_out)
{
    /* Binary search for d.  All arithmetic involving products uses 128‑bit
       to avoid overflow. */
    idx_t lo = 0, hi = 1;
    for (;;) {
        __uint128_t t = (__uint128_t)hi * (hi + 1) / 2;
        if (t > n) break;
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n) lo = mid;
        else hi = mid - 1;
    }
    idx_t d = lo;
    idx_t start = d * (d + 1) / 2;
    idx_t i = n - start;
    idx_t j = d - i;

    *d_out = d;
    *i_out = i;
    *j_out = j;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t d, i, j;
    diag_of(n, &d, &i, &j);

    seq_t val = 1;          /* start from T(0,0) */

    /* First build the entry in column j from T(0,0) */
    for (idx_t k = 1; k <= j; ++k) {
        /* T(0,k) = T(0,k-1) * (64*k-56) / k */
        val = mul_div(val, 64 * k - 56, k);
    }

    /* Then move down to row i along that column */
    for (idx_t r = 1; r <= i; ++r) {
        /* T(r,j) = T(r-1,j) * (64*r-8) / (r + j) */
        val = mul_div(val, 64 * r - 8, r + j);
    }

    return val;
}
