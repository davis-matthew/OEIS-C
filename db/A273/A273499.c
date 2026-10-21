/*
 * A273499  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 771", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)^2 - 2^(floor(log2(n)) + popcount(n) + 1), with
 *            a(0) = 1. Here popcount(n) is the number of 1‑bits in n
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the floor of log2(v) for v > 0. */
static inline idx_t floor_log2(idx_t v) {
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Count the number of set bits in v. */
static inline idx_t popcount(idx_t v) {
    idx_t c = 0;
    while (v) { v &= v - 1; ++c; }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t tn = (seq_t)n;
    seq_t term = (2 * tn + 1);
    term = term * term;

    idx_t e = floor_log2(n) + popcount(n) + 1;
    seq_t diff = ((seq_t)1) << e;
    return term - diff;
}
