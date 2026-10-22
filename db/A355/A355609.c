/*
 * A355609  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. (1 - x)^(-x^k).
 * https://oeis.org/A355609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where (row,col) are the antidiagonal
 *            coordinates of n; T(n,k) = n! * Σ_{j=0..⌊n/(k+1)⌋} |s(n-k*j, j)|
 *            / (n-k*j)!, where s(m,j) are unsigned Stirling numbers of the
 *            first kind
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 664, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/antidiagonal_coords.h"

static size_t curMaxRow = 0;          /* largest computed row */
static seq_t *fact = NULL;            /* factorial[0..curMaxRow] */
static seq_t **stirling = NULL;       /* stirling[n][j] for 0<=j<=n */
static seq_t **Ttable = NULL;         /* Ttable[n][k] for 0<=k<=n */

/* Ensure all tables up to 'row' are allocated and filled. */
static void ensure_tables(size_t row)
{
    if (row <= curMaxRow) return;

    /* ---------- factorials ---------- */
    fact = (seq_t *)realloc(fact, (row + 1) * sizeof(seq_t));
    if (curMaxRow == 0) fact[0] = (seq_t)1;        /* init 0! */
    for (size_t i = curMaxRow + 1; i <= row; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* ---------- Stirling numbers (first kind) ---------- */
    stirling = (seq_t **)realloc(stirling, (row + 1) * sizeof(seq_t *));
    /* allocate new rows */
    for (size_t n = curMaxRow + 1; n <= row; ++n) {
        stirling[n] = (seq_t *)calloc(n + 1, sizeof(seq_t));
    }
    /* initialise row 0 if not done yet */
    if (curMaxRow == 0) {
        if (!stirling[0]) {
            stirling[0] = (seq_t *)calloc(1, sizeof(seq_t));
        }
        stirling[0][0] = (seq_t)1;   /* s(0,0)=1 */
    }
    /* compute Stirling numbers for new rows */
    for (size_t n = curMaxRow + 1; n <= row; ++n) {
        /* s(n,0)=0 for n>0 */
        if (n > 0) stirling[n][0] = (seq_t)0;
        for (size_t j = 1; j <= n; ++j) {
            seq_t part1 = (j - 1 <= n - 1) ? stirling[n - 1][j - 1] : (seq_t)0;
            seq_t part2 = (j <= n - 1)       ? (seq_t)(n - 1) * stirling[n - 1][j] : (seq_t)0;
            stirling[n][j] = part1 + part2;
        }
    }

    /* ---------- T table ---------- */
    Ttable = (seq_t **)realloc(Ttable, (row + 1) * sizeof(seq_t *));
    for (size_t n = curMaxRow + 1; n <= row; ++n) {
        Ttable[n] = (seq_t *)calloc(n + 1, sizeof(seq_t));
    }
    for (size_t n = curMaxRow + 1; n <= row; ++n) {
        for (size_t k = 0; k <= n; ++k) {
            /* Special handling for n==0: all columns are 1 */
            if (n == 0) {
                Ttable[0][0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            size_t max_j = n / (k + 1);      /* floor(n/(k+1)) */
            for (size_t j = 0; j <= max_j; ++j) {
                size_t m = n - k * j;        /* m = n - k*j */
                if (m > n) continue;         /* safety, should not happen */
                seq_t stir = stirling[m][j];
                if (stir == (seq_t)0) continue;
                seq_t part = fact[n] / fact[m];   /* n! / m! */
                sum += stir * part;
            }
            Ttable[n][k] = sum;
        }
    }

    curMaxRow = row;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);          /* (row,column) of the anti‑diagonal */
    size_t row = (size_t)i;
    size_t col = (size_t)j;

    if (row == 0) return (seq_t)1;           /* T(0,k)=1 for all k */
    if (col > row) return (seq_t)0;          /* k>n ⇒ 0 */

    ensure_tables(row);                      /* compute all rows up to 'row' */

    return Ttable[row][col];
}
