/*
 * A364842  Table read by antidiagonals: row n gives the Euler transform of
 *          the sequence (2,...,2,0,0,...) that contains n 2's followed by
 *          0's.
 * https://oeis.org/A364842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Euler transform of the sequence (2,…,2,0,0,…) * with n
 *            twos, read from the infinite table by antidiagonals
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Precomputed table of Euler transform coefficients.
 * B[r][k] = coefficient of x^k in ∏_{i=1}^{r} (1 - x^i)^(-2).
 * Row r corresponds to the row with r twos in the definition. */
static seq_t B[251][251];
static bool init_done = false;

/* Initialize B[ ][ ] for all r ≤ 250 and k ≤ 250.  */
static void init_table(void)
{
    for (size_t r = 0; r <= 250; ++r)
        for (size_t k = 0; k <= 250; ++k)
            B[r][k] = 0;

    B[0][0] = 1;                     /* empty product gives 1. */

    for (size_t r = 1; r <= 250; ++r) {
        for (size_t t = 0; t <= 250; ++t) {
            seq_t sum = 0;
            for (size_t m = 0; m * r <= t; ++m) {
                sum += B[r - 1][t - m * r] * (seq_t)(m + 1);
            }
            B[r][t] = sum;
        }
    }
    init_done = true;
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (!init_done)
        init_table();

    /* Find diagonal d: d(d+1)/2 < n ≤ (d+1)(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n)
        ++d;

    idx_t before = d * (d + 1) / 2;           /* number of terms before diagonal d */
    idx_t pos    = n - before - 1;           /* position within diagonal d (0‑based) */
    idx_t r      = d - pos + 1;              /* row index (1‑based)          */
    idx_t k      = pos;                      /* column index (0‑based)       */

    return B[(size_t)r][(size_t)k];
}
