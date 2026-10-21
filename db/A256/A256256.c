/*
 * A256256  Total number of ON cells after n generations of cellular automaton
 *          on triangular grid, starting from a node, in which every 60-degree
 *          wedge looks like the Sierpiński's triangle.
 * https://oeis.org/A256256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*A006046(n) = 6 * sum_{i=0}^{n-1} 2^{popcount(i)}
 * Range:     u8 n <= 11, u16 n <= 383, u32 n <= 9356, u64 n <= 9356,
 *            u128 n <= 9356, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any non‑negative n. */

#include <stdint.h>
#include <stddef.h>

/* Helper to compute 3^exp as a seq_t.  exp <= 63 for 64‑bit indices. */
static seq_t pow3_exp(size_t exp) {
    seq_t r = 1;
    for (size_t i = 0; i < exp; ++i)
        r *= 3;
    return r;
}

/* Recursive computation of S(n) = sum_{i=0}^{n-1} 2^{popcount(i)}. */
static seq_t sum_pow2_popcount(idx_t n) {
    if (n == 0)
        return 0;               /* empty sum */
    if (n == 1)
        return 1;               /* only i = 0 : 2^0 = 1 */
    /* Find the largest power of two < n. */
    idx_t k = 63 - __builtin_clzll(n - 1);          /* floor(log2(n-1)) */
    idx_t base = (idx_t)1 << k;                     /* 2^k */
    seq_t first_part = pow3_exp((size_t)k);         /* sum over 0 .. base-1 */
    idx_t lower = n - base;                         /* remaining elements */
    return first_part + ((seq_t)2 * sum_pow2_popcount(lower));
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    return (seq_t)6 * sum_pow2_popcount(n);
}
