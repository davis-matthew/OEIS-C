/*
 * A278136  Triangle read by rows: T(n,k) is the maximum number of disjoint
 *          subgraphs of the Fibonacci cube Gamma(n) that are isomorphic to
 *          the hypercube of dimension k.
 * https://oeis.org/A278136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 154, u32 n <= 574, u64 n <= 2207,
 *            u128 n <= 2650, big any n
 * Helpers:   binom, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A278136.  The sequence is obtained by reading the
   triangle T(n,k) row by row.  For each flattened index n we determine
   its row r and column k, then evaluate T(r,k) by the closed‑form
   summation with Fibonacci numbers and binomial coefficients. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/binom.h"

static seq_t
compute_T(idx_t n, idx_t k)
{
    if (k == 0) return fib(n + 2);
    idx_t i_min = k - 1;
    idx_t i_max = (n + k - 2) / 3;          /* floor((n+k-2)/3) */
    seq_t sum = 0;
    for (idx_t i = i_min; i <= i_max; ++i) {
        seq_t coeff   = binom(i, k - 1);
        idx_t fib_idx = n + k - 3 * i - 1;
        seq_t term = coeff * fib(fib_idx);
        sum += term;
    }
    return sum;
}

seq_t
a(idx_t n)
{
    /* Locate the row and column corresponding to the flat index n. */
    idx_t row = 0;
    idx_t cum = 0;
    while (1) {
        idx_t len = 1 + ((row + 1) / 2);   /* 1 + ceil(row/2) */
        if (cum + len > n) break;
        cum += len;
        ++row;
    }
    idx_t k = n - cum;
    return compute_T(row, k);
}
