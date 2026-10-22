/*
 * A360849  Array read by antidiagonals: T(m,n) is the number of (undirected)
 *          cycles in the complete bipartite graph K_{m,n}.
 * https://oeis.org/A360849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m, n) where the pair (m, n) is defined by the
 *            antidiagonal ordering of the array T(m, n). For a given index n
 *            (with n ≥ 1) let d be the smallest integer ≥ 2 such that
 *            d(d‑1)/2 ≥ n. Then let offset = n – (d‑2)(d‑1)/2, which is
 *            1‑based within the d‑th diagonal. The coordinates are m =
 *            offs...
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 143, u64 n <= 337,
 *            u128 n <= 880, big any n
 * Helpers:   _diag_for_n, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/_diag_for_n.h"

static seq_t _T_value(idx_t m, idx_t n) {
    idx_t mn = m < n ? m : n;
    if (mn < 2)
        return (seq_t)0;
    seq_t sum = 0;
    for (idx_t j = 2; j <= mn; ++j) {
        seq_t term = binomial(m, j);
        term *= binomial(n, j);
        term *= factorial(j);
        term *= factorial(j - 1);
        term /= (seq_t)2;
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;
    idx_t d = _diag_for_n(n);
    unsigned __int128 tri_prev = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t offset = n - (idx_t)tri_prev;      /* 1‑based position in the diagonal */
    idx_t m = offset;
    idx_t NN = d - offset;
    return _T_value(m, NN);
}
