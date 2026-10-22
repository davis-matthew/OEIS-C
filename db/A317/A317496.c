/*
 * A317496  Triangle T(n,k) = T(n-1,k) + 3*T(n-3,k-1) for k = 0..floor(n/3)
 *          with T(0,0) = 1, T(n,k) = 0 for n or k < 0, read by rows.
 * https://oeis.org/A317496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = 3^k * C(n-2k, k) where n is index in the flattened
 *            triangle, k = n - Σ_{i=0}^{r-1} (⌊i/3⌋+1) and r is the smallest
 *            row such that Σ_{i=0}^{r} (⌊i/3⌋+1) > n
 * Range:     u8 n <= 28, u16 n <= 88, u32 n <= 281, u64 n <= 991,
 *            u128 n <= 2500, big any n
 * Helpers:   binom_multip
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_multip.h"

/* compute 3^k for small k */
static inline seq_t pow3_idx(idx_t k) {
    seq_t r = 1;
    for (idx_t i = 0; i < k; ++i) {
        r *= 3;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Find the row r and column k of the nth element in the
       flattened triangle */
    idx_t row = 0;
    idx_t len = 1;            /* length of current row = ⌊row/3⌋+1 */
    idx_t start = 0;          /* index of first element of current row */

    while (start + len <= n) {
        start += len;
        ++row;
        len = row / 3 + 1;   /* next row length */
    }

    idx_t k = n - start;                /* column index */
    seq_t val = pow3_idx(k) * binom_multip(row - 2 * k, k);
    return val;
}
