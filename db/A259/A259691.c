/*
 * A259691  Triangle read by rows: T(n,k) number of arrangements of
 *          non-attacking rooks on an n X n right triangular board where the
 *          top rook is in row k (n >= 0, 1 <= k <= n+1).
 * https://oeis.org/A259691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 135, u64 n <= 350,
 *            u128 n <= 945, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the flattened right‑triangular rook‑board numbers of A259691.
   Each term a(n) is T(r,k) where r satisfies triangular(r) ≤ n < triangular(r+1)
   and k = n - triangular(r) + 1.  For 1 ≤ k ≤ r, 
   T(r,k) = k * ∑_{i=0}^{r-k} binomial(r-k,i) * k^i * Bell(r-k-i),
   with T(r,r+1) = 1.  The code evaluates the formula directly. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"            /* seq_t binomial(idx_t a, idx_t b); */

/* -------------------------------------------------------------------------- */
/* Helper: triangular number T(r) = r*(r+1)/2 stored as uint64_t.              */
/* -------------------------------------------------------------------------- */
static inline idx_t triangular(idx_t r)
{
    /* use 128‑bit intermediate to avoid overflow of 64‑bit arithmetic   */
    __uint128_t t = (__uint128_t)r * (r + 1) / 2;
    return (idx_t)t;
}

/* -------------------------------------------------------------------------- */
/* Helper: find largest r with T(r) <= n.                                     */
/* -------------------------------------------------------------------------- */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search to bracket the value            */
    while (triangular(high) <= n) high <<= 1;
    /* binary search inside the bracket                  */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* -------------------------------------------------------------------------- */
/* Compute Bell numbers B[0..m] in a small local array (size 201 is enough).*/
/* -------------------------------------------------------------------------- */
static void bell_up_to(idx_t m, seq_t bell[/*201*/])
{
    bell[0] = (seq_t)1;
    for (idx_t n = 1; n <= m; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= n - 1; ++k) {
            seq_t bin = binomial(n - 1, k);
            sum += bin * bell[k];          /* uses overloaded operators */
        }
        bell[n] = sum;
    }
}

/* -------------------------------------------------------------------------- */
/* Main OEIS sequence generator for A259691 (flattened triangle).           */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* determine row r and column k (1‑based) */
    idx_t   r = find_row(n);
    idx_t   k = n - triangular(r) + 1;

    /* special top‑rook‑absent case */
    if (k == r + 1)
        return (seq_t)1;

    /* delta = r - k  (≥ 0) */
    idx_t delta = r - k;

    /* compute Bell numbers up to delta once */
    seq_t bell[201];
    bell_up_to(delta, bell);

    /* accumulate the summation */
    seq_t sum      = (seq_t)0;
    seq_t pow_k    = (seq_t)1;          /* k^0 */
    for (idx_t i = 0; i <= delta; ++i) {
        seq_t bin   = binomial(delta, i);
        seq_t term  = bin * pow_k * bell[delta - i];
        sum += term;
        pow_k *= (seq_t)k;              /* advance k^(i+1) */
    }

    return (seq_t)k * sum;
}
