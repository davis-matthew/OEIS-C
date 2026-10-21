/*
 * A241669  Irregular triangular array read by rows: T(n,k) is the number of
 *          2-colored simple labeled graphs on n nodes that have exactly k
 *          edges, 0<=k<=A002620(n), n>=1.
 * https://oeis.org/A241669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Σ_{s=1}^{n-1} C(n,s) * C(s*(n-s), k)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the flattened triangle where row r has length floor(r²/4)+1 */
/* and the k‑th element of that row (k=0 … floor(r²/4)) is T(r,k). */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t total = 0;
    idx_t row = 0;

    /* Find the row that contains the n‑th element */
    while (1) {
        ++row;
        idx_t len = (row * row) / 4 + 1;           /* number of elements in this row */
        if (n <= total + len) {
            idx_t k = n - total - 1;              /* column index (0‑based) */

            /* Compute T(row, k) */
            seq_t sum = 0;
            for (idx_t s = 1; s < row; ++s) {
                seq_t comb1 = binom(row, s);
                idx_t e = s * (row - s);
                if (k > e) continue;              /* binomial is zero in this case */
                seq_t comb2 = binom(e, k);
                sum += comb1 * comb2;
            }
            return sum;
        }
        total += len;
    }
}
