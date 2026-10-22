/*
 * A372868  Irregular triangle read by rows: T(n,k) is the number of flattened
 *          Catalan words of length n with exactly k runs of weak ascents,
 *          with 1 <= k <= ceiling(n/2).
 * https://oeis.org/A372868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of the concatenated irregular triangle
 *            T(n,k) = 2^(n-2*k+1) * binomial(n-1, 2*k-2). For an index n,
 *            find the row r such that Σ_{m<r} ceil(m/2) < n ≤ Σ_{m≤r}
 *            ceil(m/2). Let c = n - Σ_{m<r} ceil(m/2). Then a(n) = T(r,c)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for idx_t, seq_t definition */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Locate the row r and column c for the n-th term */
    idx_t cum = 0;   /* cumulative terms up to the previous row */
    idx_t r = 1;     /* current row */

    while (1) {
        idx_t len = (r + 1) / 2;          /* ceil(r/2) */
        if (cum + len >= n) break;
        cum += len;
        ++r;
    }

    idx_t c = n - cum;                    /* column index, 1‑based */

    /* Compute the term: T(r,c) = 2^(r-2*c+1) * binomial(r-1, 2*c-2) */
    seq_t p2 = pow2(r - 2 * c + 1);
    seq_t bin = binomial(r - 1, 2 * c - 2);
    return p2 * bin;
}
