/*
 * A355246  Square array A(n, k), n, k >= 0, read by antidiagonals; for any m
 *          > 0, the position of the m-th rightmost 0 in the binary expansion
 *          of A(n, k) is the greatest of the positions of the m-th rightmost
 *          0 in the binary expansions of n and k (the least significant bit
 *          having position 0).
 * https://oeis.org/A355246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A355246.
   a(n) is the entry A(i,j) of the square array defined by the rule that
   for each bit position p, the m‑th rightmost 0 of A(i,j) is the maximum
   of the m‑th rightmost 0 of i and j.  The pair (i,j) is obtained by
   reading the array in antidiagonals:  a(0)=A(0,0), a(1)=A(0,1), a(2)=A(1,0), … */

#include <stdint.h>
#include <stdbool.h>

/* Convert index n to coordinates (i,j) on the antidiagonal ordering. */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t s = 0;
    /* total entries up to diagonal s: (s+1)(s+2)/2 */
    idx_t total = (s + 1) * (s + 2) / 2;
    while (total <= n) {
        ++s;
        total = (s + 1) * (s + 2) / 2;
    }
    idx_t prev_total = s * (s + 1) / 2;   /* entries before diagonal s */
    idx_t d = n - prev_total;             /* position on this diagonal */
    *i = d;
    *j = s - d;
}

/* Compute A(i,j) according to the zero‑position rule. */
static inline seq_t A_value(idx_t i, idx_t j)
{
    idx_t rem_i = i, rem_j = j;
    seq_t result = 0;
    seq_t zeros_i = 0, zeros_j = 0;   /* cumulative counts of zeros */
    seq_t prev_min = 0;
    idx_t pos = 0;

    while (rem_i || rem_j) {
        bool nz_i = (rem_i & 1ULL) == 0;     /* bit 0? */
        bool nz_j = (rem_j & 1ULL) == 0;
        if (nz_i) ++zeros_i;
        if (nz_j) ++zeros_j;
        seq_t cur_min = zeros_i < zeros_j ? zeros_i : zeros_j;

        if (cur_min == prev_min) {
            /* Bit at position pos is 1. */
            result |= ((seq_t)1 << pos);
        }
        prev_min = cur_min;

        rem_i >>= 1;
        rem_j >>= 1;
        ++pos;
    }
    return result;
}

/* Public interface required by the problem statement. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return A_value(i, j);
}
