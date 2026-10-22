/*
 * A371395  Triangle read by rows: T(n, k) = binomial(n + k, k) * binomial(2*n
 *          - k, n - k) / (n + 1).
 * https://oeis.org/A371395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th term of the triangle read by rows. * The
 *            triangle T(n,k) is defined by * T(n,k) = binomial(n+k , k) *
 *            binomial(2*n-k , n-k) / (n+1) * for 0 ≤ k ≤ n, n ≥ 0. * The
 *            sequence a(n) is the row‑major ordering of T(n,k)
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _prefix_of_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_prefix_of_row.h"

/* -------------------------------------------------------------------------- */
/*  Helper: binomial coefficient (multiplicative)                             */
/* -------------------------------------------------------------------------- */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i, exact division */
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        res = (res * num) / den;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
/*  Prefix sum of row counts (triangular numbers)                            */

/* -------------------------------------------------------------------------- */
/*  Given a linear index n, find the row r such that
 *           prefix_of_row(r) ≤ n < prefix_of_row(r+1)
 * -------------------------------------------------------------------------- */
static idx_t _find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* grow high until it exceeds n */
    while (_prefix_of_row(high) <= n) high <<= 1;
    /* binary search */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (_prefix_of_row(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* -------------------------------------------------------------------------- */
/*  Sequence entry a(n)                                                       */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* locate the row and column (within that row) of the n-th term */
    idx_t r = _find_row(n);
    idx_t base = _prefix_of_row(r);   /* first index of this row */
    idx_t c = n - base;               /* column index within the row */

    /* compute T(r,c) = binom(r+c,c) * binom(2*r - c, r - c) / (r+1) */
    seq_t first  = _binom(r + c, c);
    seq_t second = _binom(2 * r - c, r - c);
    seq_t val    = first * second;

    val /= (seq_t)(r + 1);            /* division by (r+1) is exact */
    return val;
}
