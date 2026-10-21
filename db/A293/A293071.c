/*
 * A293071  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of g.f. Product_{i>0} Sum_{j=0..k}
 *          (-1)^j*j!*x^(j*i).
 * https://oeis.org/A293071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   not stated
 * Range:     i8 n <= 83, i16 n <= 143, i32 n <= 363, i64 n <= 923,
 *            i128 n <= 2379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A293071
   Whenever a(idx_t n) is called it returns the n‑th term of the
   sequence obtained by reading the square array A(n,k) (rows n ≥ 0,
   columns k ≥ 0) by antidiagonals.  The column k has g.f.  
   ∏_{i>0}  Σ_{j=0..k} (-1)^j · j! · x^{i·j}.
   For a given n we compute
       t = ⌊ (√(8·n+1) – 1) / 2 ⌋
       k = n – t·(t+1)/2          // row index (exponent)
       j = t – k                  // column index
   then a(n) = A[k][j] (coefficient of x^k in column j).  The
   routine builds the needed part of the array once, stores it in
   a small static table and answers all further calls from that
   table.  The maximum required indices for n ≤ 10 000 are
   0 … 199, so a table of size 200×200 is sufficient.  All
   arithmetic is done in the type seq_t, which may be any signed
   integer or big‑int type.  No external helper functions are used. */

#include <stdint.h>
#include <stdbool.h>

/* integer (unsigned) square root floor for 64‑bit values   */
static idx_t floor_sqrt_u64(idx_t v)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;          /* highest power of two ≤ 2^63 */
    while (bit) {
        idx_t nxt = r + bit;
        if (nxt <= v / nxt) r = nxt;
        bit >>= 1;
    }
    return r;
}

/* maximum index that can appear in the array for n up to about 10 000
   (t = ⌊(√(8·n+1) – 1)/2⌋ ≤ 199).  Definitions fit for all
   tested values.                                                     */
#define MAX_IDX 200   /* sufficient for n ≈ 20000, more is harmless   */
static seq_t table[MAX_IDX + 1][MAX_IDX + 1];
static bool table_built = false;

/* Build the table table[i][j] = coefficient of x^i in column j. */
static void build_table(void)
{
    if (table_built) return;

    /* factorials up to MAX_IDX (needed for coefficients (-1)^r r!). */
    seq_t fact[MAX_IDX + 1];
    fact[0] = (seq_t)1;
    for (idx_t r = 1; r <= MAX_IDX; ++r) fact[r] = fact[r - 1] * (seq_t)r;

    /* For every column j compute its coefficient array up to degree
       MAX_IDX (exponent i).  Each column uses its own DP array. */
    for (idx_t j = 0; j <= MAX_IDX; ++j) {
        /* coefficients w[r] = (-1)^r · fact[r]  (r = 0..j). */
        seq_t w[MAX_IDX + 1];
        for (idx_t r = 0; r <= j; ++r) {
            w[r] = (r & 1) ? -fact[r] : fact[r];
        }

        /* dp[n] = coefficient of x^n after processing all part sizes. */
        seq_t dp[MAX_IDX + 1];
        seq_t newdp[MAX_IDX + 1];

        for (idx_t n = 0; n <= MAX_IDX; ++n) dp[n] = (n == 0) ? (seq_t)1 : 0;

        /* Loop over part sizes i (here called p). */
        for (idx_t p = 1; p <= MAX_IDX; ++p) {
            for (idx_t n = 0; n <= MAX_IDX; ++n) newdp[n] = 0;

            for (idx_t n = 0; n <= MAX_IDX; ++n) {
                seq_t sum = 0;
                /* r runs from 0 up to j but we stop when p·r > n. */
                for (idx_t r = 0; r <= j; ++r) {
                    idx_t shift = p * r;
                    if (shift > n) break;
                    sum += dp[n - shift] * w[r];
                }
                newdp[n] = sum;
            }
            /* swap dp and newdp */
            for (idx_t n = 0; n <= MAX_IDX; ++n) dp[n] = newdp[n];
        }

        /* Store results into table: row = exponent i = n. */
        for (idx_t i = 0; i <= MAX_IDX; ++i) {
            table[i][j] = dp[i];
        }
    }
    table_built = true;
}

seq_t a(idx_t n)
{
    /* Build table on first call.  Subsequent calls reuse it. */
    build_table();

    /* Find antidiagonal indices. */
    idx_t t = (floor_sqrt_u64((idx_t)8 * n + 1) - 1) / 2;

    idx_t k = n - t * (t + 1) / 2;   /* row index (exponent) */
    idx_t j = t - k;                 /* column index */

    /* Bounds are guaranteed to be ≤ MAX_IDX from construction. */
    return table[k][j];
}
