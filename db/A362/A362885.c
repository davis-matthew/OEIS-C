/*
 * A362885  Array read by ascending antidiagonals: A(n, k) = (1 + 2*n)*k^n.
 * https://oeis.org/A362885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value read by ascending antidiagonals of the array A(n,k)
 *            = (1 + 2*n)*k^n
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS sequence A362885. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal index t such that
       C(t-1) <= n < C(t) where C(t) = (t+1)(t+2)/2. */
    uint64_t m = (uint64_t)n;           /* n fits in 64‑bit. */
    uint64_t t = 0;
    uint64_t cum_next = 1;             /* C(0) = 1. */
    while (cum_next <= m) {
        t++;
        cum_next = (t + 1) * (t + 2) / 2;
    }
    /* C(t-1) = cum_next - (t + 1). */
    uint64_t cum_prev = cum_next - (t + 1);
    /* Position within the antidiagonal. */
    idx_t pos = (idx_t)(m - cum_prev);
    /* Coordinates (n_val, k_val). */
    idx_t n_val = t - pos;
    idx_t k_val = pos;

    /* Compute (1 + 2*n_val) * k_val^n_val. */
    seq_t result = 1;
    for (idx_t i = 0; i < n_val; ++i) {
        result *= (seq_t)k_val;
    }
    result *= (seq_t)(1 + 2 * n_val);
    return result;
}
