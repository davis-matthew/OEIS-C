/*
 * A276321  List of B-spline interpolation matrices M(n,i,j) of orders n >= 1
 *          read by rows (0 <= i < n, 0 <= j < n).
 * https://oeis.org/A276321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = row‑wise flattening of the B‑spline interpolation
 *            matrices M(k,i,j) given by * M(k,i,j) = C(k-1,i) * Σ_{m=j}^{k-1}
 *            [ (k-(m+1))^i * (-1)^(m-j) * C(k,m-j) ]. * Sequence starts at
 *            n=1, corresponding to the first entry of M(1,0,0)
 * Range:     i8 n <= 114, i16 n <= 348, i32 n <= 1007, i64 n <= 3782,
 *            i128 n <= 9156, big any n
 * Helpers:   binomial, cum_sq, find_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/cum_sq.h"
#include "../utils/find_k.h"

/* binary exponentiation for seq_t.  exp fits in idx_t. */
static inline seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n = 0, return 0 for safety */

    /* Determine the order k of the matrix that contains the n‑th term. */
    idx_t k = find_k(n);

    /* Compute previous cumulative count and local position. */
    idx_t prev = (k > 1) ? (idx_t)cum_sq(k - 1) : 0;
    idx_t pos  = n - prev - 1;      /* zero‑based within the k×k matrix */

    /* Row i and column j (read row‑wise). */
    idx_t i = pos / k;
    idx_t j = pos % k;

    /* Pre‑compute the outer binomial coefficient C(k-1,i). */
    seq_t coeff = binomial(k - 1, i);

    /* Accumulate the sum over m = j … k-1. */
    seq_t total = 0;
    for (idx_t m = j; m < k; ++m) {
        seq_t base_num = (seq_t)(k - (m + 1));        /* k-(m+1) */
        seq_t base_pow = seq_pow(base_num, i);       /* (k-(m+1))^i */
        seq_t sign = ((m - j) & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^(m-j) */
        seq_t binom = binomial(k, m - j);            /* C(k, m-j) */
        seq_t term = base_pow * sign * binom;
        total += term;
    }

    return coeff * total;
}
