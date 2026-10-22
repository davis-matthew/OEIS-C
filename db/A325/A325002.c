/*
 * A325002  Triangle read by rows: T(n,k) is the number of oriented colorings
 *          of the facets (or vertices) of a regular n-dimensional simplex
 *          using exactly k colors.
 * https://oeis.org/A325002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangle read by rows, where T(r,k)
 *            = binomial(r,k-1) + [k==r+1] for 1 ≤ k ≤ r+1
 * Range:     u8 n <= 6, u16 n <= 178, u32 n <= 540, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function calculates the appropriate row r and column k
   from the index n (1‑based), then evaluates T(r,k). */

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the smallest r such that r*(r+3)/2 ≥ n. */
    seq_t s = (seq_t)9 + (seq_t)8 * (seq_t)n;
    seq_t root = floor_sqrt(s);
    /* Initial approximation of r (floor of (-3+sqrt(s))/2). */
    seq_t r = (root > 3) ? ((root - (seq_t)3) / (seq_t)2) : (seq_t)0;
    /* Correct r upward until the inequality holds. */
    while ( ((r * ((r + 3))) / 2) < n ) {
        ++r;
    }
    /* Compute offset within the found row. */
    seq_t prev = ((r - 1) * (r + 2)) / 2;   /* terms before row r */
    seq_t offset = (seq_t)n - prev;         /* 1‑based column index k */
    idx_t k = (idx_t)offset;                /* k ranges 1 … r+1 */
    /* Evaluate T(r,k). */
    seq_t val = binomial((idx_t)r, k - 1);
    if (k == (idx_t)(r + 1)) {
        val += (seq_t)1;
    }
    return val;
}
