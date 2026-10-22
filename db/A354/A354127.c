/*
 * A354127  Triangle read by rows: T(n, k) is the number of graphs obtained by
 *          adding k pierced circles to a path graph P_n.
 * https://oeis.org/A354127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(floor((√(8n+1)-1)/2), n -
 *            floor((√(8n+1)-1)/2)*(floor((√(8n+1)-1)/2)+1)/2) where T(n,k) =
 *            Σ_{m=k..n} (-1)^{m+k} * binomial(m,k) * binomial(2*n-m-1, m) *
 *            Catalan(n-m)^2 (cf. the OEIS A354127 triangle description)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* Convert a global index in the flattened triangle to row and column. */
static inline idx_t row_from_index(idx_t idx) {
    if (idx == 0ULL) return 0ULL;
    long double discr = sqrtl(1.0L + 8.0L * (long double)idx);
    idx_t row = (idx_t)((discr - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((row + 1ULL) * (row + 2ULL) / 2ULL <= idx) ++row;
    while (row * (row + 1ULL) / 2ULL > idx) --row;
    return row;
}

/* Compute the triangle entry T(n,k). */
static seq_t compute_T(idx_t n, idx_t k) {
    seq_t sum = 0;
    for (idx_t m = k; m <= n; ++m) {
        seq_t bin_mk = binomial(m, k);
        seq_t bin_om = binomial(2 * n - m - 1, m);
        seq_t cat = catalan(n - m);
        cat = cat * cat;                 /* Catalan(n-m)^2 */
        seq_t term = bin_mk * bin_om;
        term = term * cat;
        if (((m + k) & 1ULL) == 0ULL)
            sum += term;                 /* positive sign */
        else
            sum -= term;                 /* negative sign */
    }
    return sum;
}

/* Public sequence function. */
seq_t a(idx_t flat_idx) {
    if (flat_idx == 0ULL) return 1;     /* T(0,0) = 1 */

    idx_t n_row = row_from_index(flat_idx);
    idx_t tri = n_row * (n_row + 1ULL) / 2ULL;
    idx_t k_col = flat_idx - tri;

    return compute_T(n_row, k_col);
}
