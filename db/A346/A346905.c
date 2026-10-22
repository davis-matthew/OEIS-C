/*
 * A346905  Triangle read by rows: T(n,k) is the number of ways of choosing a
 *          k-dimensional cube from the vertices of an n-dimensional
 *          hypercube; 0 <= k <= n.
 * https://oeis.org/A346905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) with m and k chosen so that n = m(m+1)/2 + k, 0 ≤
 *            k ≤ m. T(m,k) = 2^(m-k) * sum_{i=1..floor(m/k)} m! / (k! ·
 *            (i!)^k · (m-i·k)!)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types supplied by the build are used. */
#include <stdbool.h>

/* ------------------------------------ helpers ------------------------------------ */

/* Power of 2: 2^e as seq_t.  Handles any integer width of seq_t. */
static seq_t pow2_seq(idx_t e)
{
    seq_t res = 1;
    /* shift works for arbitrary sized seq_t too */
    res <<= e;
    return res;
}

/* Factorial: n! as seq_t.  For large n the value may overflow the type
   but that is fine – we only need exact results if they fit. */
static seq_t factorial(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;
    return res;
}

/* Binomial coefficient C(n,k) as seq_t, computed multiplicatively to keep
   intermediate values as small as possible.  Assumes 0 ≤ k ≤ n. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n - k)   /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;  all divisions are exact */
        res = res * (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Power as seq_t: base^exp (exp fits in idx_t). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* ------------------------------------ main function ------------------------------------ */

seq_t a(idx_t n)
{
    /* Find the row m such that m(m+1)/2 > n.  Then the row index is m-1. */
    idx_t m = 0;
    while ((unsigned __int128)m * (m + 1u) / 2 <= n)
        ++m;
    idx_t row = m - 1;

    /* Number of terms before this row: row(row+1)/2. */
    idx_t prev_terms = (unsigned __int128)row * (row + 1u) / 2;
    idx_t col = n - prev_terms;   /* k */

    /* Case k == 0: T(n,0) = 2^n. */
    if (col == 0)
        return pow2_seq(row);

    /* General case: compute the sum. */
    seq_t sum = 0;
    idx_t max_i = row / col;
    seq_t fact_k = factorial(col);

    for (idx_t i = 1; i <= max_i; ++i) {
        /* term = binomial(row, i*col) */
        seq_t term = binomial(row, i * col);

        /* Multiply by product_{r=1..col} binomial(i*r, i) part */
        for (idx_t r = 1; r <= col; ++r) {
            term *= binomial(i * r, i);
        }

        /* Divide by k! .  The division is exact by definition. */
        term /= fact_k;

        sum += term;
    }

    /* Multiply by 2^(row-col). */
    seq_t power = pow2_seq(row - col);
    return power * sum;
}
