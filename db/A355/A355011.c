/*
 * A355011  Array read by ascending antidiagonals: T(n, k) is the number of
 *          self-conjugate n-core partitions with k corners.
 * https://oeis.org/A355011
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is obtained by reading the array
 *            T(n,k) in ascending antidiagonals (offset 2). T(n,k) is defined
 *            by the binomial sum given in the OEIS entry
 * Range:     u8 n <= 70, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Local helper that evaluates T(i,j) for i>=2, j>=1. */
static seq_t T(idx_t i, idx_t j) {
    idx_t i_half = i / 2;
    idx_t j_half = j / 2;
    idx_t t_max  = j < i_half ? j : i_half;   /* min(j, floor(i/2)) */

    seq_t sum = 0;

    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t a1 = binom((j - 1) / 2, (t - 1) / 2);
        seq_t a2 = binom(j / 2, t / 2);
        seq_t a3 = binom(i / 2 + j - t, j);

        seq_t term = a1;
        term *= a2;
        term *= a3;
        sum += term;
    }
    return sum;
}

/* Computes a(n) from the OEIS offset 2. */
seq_t a(idx_t n) {
    if (n < 2)
        return 0;                 /* behaviour below offset irrelevant */

    /* 0‑based rank within the antidiagonal ordering */
    idx_t m = n - 2;

    /* Find the diagonal sum s with C(s) > m, where C(s) = (s-2)(s-1)/2. */
    idx_t s = 3;
    __int128 cum = (__int128)(s - 2) * (s - 1) / 2;
    while (cum <= (__int128)m) {
        ++s;
        cum = (__int128)(s - 2) * (s - 1) / 2;
    }

    /* Previous cumulative count */
    idx_t cumPrev = (s - 3) * (s - 2) / 2;
    idx_t k = m - cumPrev;          /* index inside the diagonal */

    /* Coordinates in the array T(i,j) */
    idx_t i = s - 1 - k;
    idx_t j = 1 + k;

    return T(i, j);
}
