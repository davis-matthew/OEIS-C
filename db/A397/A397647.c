/*
 * A397647  Triangle read by rows: T(n, k) is the number of partitions of [n]
 *          into k nonempty endofunctions such that a fixed set of k elements
 *          are forced into k distinct blocks (one specified element per
 *          block).
 * https://oeis.org/A397647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   rising_fact, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397647 – triangle read by rows.
 *
 *  T(n, k) = ∑_{j=0..n-k} C(n-k, j) * (k‑1)_j * n^(n-k-j)
 *  where C is the binomial coefficient and (k‑1)_j is the rising factorial.
 *
 *  a(n) is the enumeration of the triangle by rows, i.e. the term
 *  appearing in row r, column k with r(r+1)/2 ≤ n < (r+1)(r+2)/2.
 *
 *  The implementation evaluates T(r,k) directly from the above formula.
 *  It uses only integer arithmetic and the supplied helper
 *  seq_pow(base, exp) which returns base^exp as a seq_t.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"    /* seq_pow(base, exp) -> seq_t */
#include "../utils/rising_fact.h"

/* Compute the binomial coefficient C(n, k) as a seq_t. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)       return (seq_t)0;
    if (k > n - k)   k = n - k;          /* symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Evaluate T(n, k) for 0 ≤ k ≤ n.  For k==0 the value is 1 when n==0
 * and 0 otherwise (no endofunctions can be formed with a forced block
 * when n>0). */
static seq_t T_value(idx_t n, idx_t k)
{
    if (k == (idx_t)0) return (n == (idx_t)0) ? (seq_t)1 : (seq_t)0;

    seq_t sum  = (seq_t)0;
    idx_t  diff = n - k;                /* n-k */

    for (idx_t j = (idx_t)0; j <= diff; ++j) {
        seq_t term = binom_calc(diff, j);
        term = term * rising_fact(k - (idx_t)1, j);
        term = term * seq_pow((seq_t)n, diff - j);
        sum += term;
    }
    return sum;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t row          = (idx_t)0;
    idx_t terms_before = (idx_t)0;

    while (terms_before + row + (idx_t)1 <= n) {
        terms_before += row + (idx_t)1;
        ++row;
    }

    idx_t col = n - terms_before;   /* column index within the row */
    return T_value(row, col);
}
