/*
 * A321477  Regular triangle read by rows: T(n,k) is the period of
 *          {A172236(k,m)} modulo n, 0 <= k <= n - 1.
 * https://oeis.org/A321477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n_row, n_col), where T(n,k) is the period of the Lucas
 *            * sequence U_m(k,−1) modulo n. The terms are obtained by
 *            traversing * the regular triangle row‑by‑row. For a given index
 *            n, we find the row r * such that r(r+1)/2 ≥ n, set k = n –
 *            r(r−1)/2 – 1, and compute the * period of the recurre...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   period_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/period_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* guard for out‑of‑range indices */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t tri = r * (r + 1) / 2;
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Column index k (0‑based) within that row. */
    idx_t sum_before = r * (r - 1) / 2;
    idx_t k = n - sum_before - 1;

    return period_mod(r, k);
}
