/*
 * A341102  T(n, k) = [n, k] - {n, k}, where [n, k] are the (unsigned)
 *          Stirling cycle numbers and {n, k} the Stirling set numbers. Table
 *          T(n, k) read by rows, for n >= 3 and 1 <= k <= n-2.
 * https://oeis.org/A341102
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n', k') where (n',k') are obtained by flattening the
 *            triangle T(m,k) = [m,k] – {m,k} in row‑major order. The first
 *            non‑zero term corresponds to n = 3. The triangle is defined for
 *            m ≥ 3, 1 ≤ k ≤ m−2
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* precomputed unsigned Stirling numbers of the first (c) and second (s) kind.
   Tables are built up to STIR_MAX. */
#define STIR_MAX 200          /* sufficient for all n ≤ 10000 */
static seq_t c[STIR_MAX + 1][STIR_MAX + 1];
static seq_t s[STIR_MAX + 1][STIR_MAX + 1];
static int precomputed = 0;

static void precompute_stirlings(void)
{
    if (precomputed)
        return;

    /* base cases */
    c[0][0] = 1;
    s[0][0] = 1;

    for (int n = 1; n <= STIR_MAX; ++n) {
        for (int k = 1; k <= n; ++k) {
            seq_t t1 = c[n - 1][k - 1];
            seq_t t2 = (seq_t)(n - 1) * c[n - 1][k];
            c[n][k] = t1 + t2;

            seq_t u1 = s[n - 1][k - 1];
            seq_t u2 = (seq_t)k * s[n - 1][k];
            s[n][k] = u1 + u2;
        }
    }
    precomputed = 1;
}

seq_t a(idx_t n)
{
    /* sequence starts at n = 3 */
    if (n < 3)
        return 0;

    /* convert term index to (row, position) in the triangle */
    idx_t pos = n - 3;          /* zero‑based position in the flattened list */
    idx_t row = 3;
    idx_t cum = 0;              /* number of terms before the current row */

    while (cum + (row - 2) <= pos) {
        cum += (row - 2);
        ++row;
    }

    idx_t offset = pos - cum;   /* zero‑based offset inside this row */
    idx_t k = offset + 1;       /* 1‑based column index inside the row */

    /* ensure tables are ready (STIR_MAX ≈ 200 covers all required n) */
    precompute_stirlings();

    /* return the difference of Stirling cycle and set numbers */
    seq_t diff = c[row][k] - s[row][k];
    return diff;
}
