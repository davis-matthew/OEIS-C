/*
 * A334165  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = exp(-1/k) * Sum_{j>=0} (k*j + 1)^n / (k^j * j!).
 * https://oeis.org/A334165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A334165 – Square array A(n,k) read by antidiagonals.
 *
 *  A(n,k) is defined recursively by
 *      A(0, k) = 1,
 *      A(n+1, k) = A(n, k) + Σ_{m=0}^{n} binom(n, m) · k^m · A(n-m, k).
 *
 *  The sequence a(n) is obtained by reading A(n,k) along the
 *  anti‑diagonals:
 *      a(n) = A(i, k) where
 *          s     = ⌊(√(8n+1) – 1)/2⌋   (diagonal index)
 *          i     = n – s(s+1)/2        (row index, 0‑based)
 *          k     = s – i + 1           (column index, 1‑based).
 *
 *  The table is pre‑computed for all n up to 200 rows and
 *  201 columns, which is sufficient for n ≤ 10 000.
 *
 *  All arithmetic is performed in the type seq_t.  No floating point,
 *  no <math.h>, and no other external utilities are used.
 */

#include <stdint.h>

#define N_MAX 200
#define K_MAX (N_MAX + 1)

/* Global tables (built once) */
static seq_t binom[N_MAX + 1][N_MAX + 1];
static seq_t table[N_MAX + 1][K_MAX + 1];

/* Build binomial coefficients and the whole table once */
static void build_tables(void)
{
    static int built = 0;
    if (built)
        return;
    built = 1;

    /* Binomial coefficients */
    for (idx_t n = 0; n <= N_MAX; ++n) {
        binom[n][0] = binom[n][n] = (seq_t)1;
        for (idx_t m = 1; m < n; ++m) {
            binom[n][m] = binom[n - 1][m - 1] + binom[n - 1][m];
        }
    }

    /* Compute table column by column */
    for (idx_t k = 1; k <= K_MAX; ++k) {
        /* k^m for m = 0..N_MAX */
        seq_t powk[N_MAX + 1];
        powk[0] = (seq_t)1;
        for (idx_t m = 1; m <= N_MAX; ++m)
            powk[m] = powk[m - 1] * (seq_t)k;

        /* A(0,k) = 1 */
        table[0][k] = (seq_t)1;

        /* Recurrence for n >= 0 */
        for (idx_t n = 0; n < N_MAX; ++n) {
            seq_t sum = (seq_t)0;
            for (idx_t m = 0; m <= n; ++m) {
                sum += binom[n][m] * table[n - m][k] * powk[m];
            }
            table[n + 1][k] = table[n][k] + sum;
        }
    }
}

/* Return a(n) – the OEIS A334165 sequence */
seq_t a(idx_t n)
{
    build_tables();

    /* Determine diagonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    idx_t prev = s * (s + 1) / 2;        /* elements before diagonal s */
    idx_t i    = n - prev;               /* row index (0‑based)   */
    idx_t k    = s - i + 1;              /* column index (1‑based) */

    return table[i][k];
}
