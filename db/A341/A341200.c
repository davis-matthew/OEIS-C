/*
 * A341200  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} j^k * (n-j)! * binomial(n,j)^2.
 * https://oeis.org/A341200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   antidiagonal_coords, binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(i,j) where (i,j) = antidiagonal_coords(n) and
 * T(i,j) = Σ_{t=0..i} t^j · (i-t)! · binomial(i,t)^2
 */

#include <stddef.h>               /* for size_t */
#include "../utils/binomial.h"    /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t a, seq_t b) */
#include "../utils/antidiagonal_coords.h" /* antidiagonal_coords */

/* Factorial cache: fact[k] = k! for 0 <= k <= MAX_FACT.
   Max supported n is MAX_FACT (≈10000). */
#define MAX_FACT 10000
static seq_t fact[MAX_FACT + 1];
static idx_t fact_size = 0;           /* highest index computed */

static void ensure_factorial(idx_t n)
{
    if (n < fact_size) return;
    if (n > MAX_FACT) return;        /* no larger values are needed */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_size = n + 1;
}

seq_t a(idx_t n)
{
    /* Determine (row, col) on the antidiagonal */
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);

    /* Pre‑compute factorials up to row */
    ensure_factorial(row);

    seq_t result = 0;
    for (idx_t t = 0; t <= row; ++t) {
        seq_t pow_term  = seq_pow((seq_t)t, col);      /* t^col */
        seq_t fact_term = fact[row - t];               /* (row-t)! */
        seq_t bin       = binomial(row, t);            /* C(row,t) */
        seq_t bin_sq    = seq_mul(bin, bin);           /* bin^2 */
        seq_t prod      = seq_mul(pow_term,
                          seq_mul(fact_term, bin_sq));
        result = result + prod;
    }
    return result;
}
