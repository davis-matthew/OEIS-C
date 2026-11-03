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
 * Validated: standard, pass, 94 terms through n = 8747 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

static size_t built = 0;              /* rows 0..built-1 are computed (row 0 counted
                                         as built before, though never allocated) */
static seq_t *fact = NULL;            /* factorial[0..curMaxRow] */
static seq_t **stirling = NULL;       /* stirling[n][j] for 0<=j<=n */
static seq_t **Ttable = NULL;         /* Ttable[n][k] for 0<=k<=n */

/* Ensure all tables up to 'row' are allocated and filled. */
static void ensure_tables(size_t row)
{
    if (row < built) return;

    /* ---------- factorials ---------- */
    seq_t *f = mem_resize(seq_t, fact, (row + 1));
    if (!f) abort();
    fact = f;
    for (size_t i = built; i <= row; ++i) {
        fact[i] = i ? fact[i - 1] * (seq_t)i : (seq_t)1;
    }

    /* ---------- Stirling numbers (first kind) ---------- */
    seq_t **s2 = (seq_t **)realloc(stirling, (row + 1) * sizeof(seq_t *));
    if (!s2) abort();
    stirling = s2;
    /* allocate new rows */
    for (size_t n = built; n <= row; ++n) {
        stirling[n] = mem_new(seq_t, n + 1);
        if (!stirling[n]) abort();
    }
    if (built == 0) stirling[0][0] = (seq_t)1;   /* s(0,0)=1 */
    /* compute Stirling numbers for new rows */
    for (size_t n = built ? built : 1; n <= row; ++n) {
        /* s(n,0)=0 for n>0 */
        if (n > 0) stirling[n][0] = (seq_t)0;
        for (size_t j = 1; j <= n; ++j) {
            seq_t part1 = (j - 1 <= n - 1) ? stirling[n - 1][j - 1] : (seq_t)0;
            seq_t part2 = (j <= n - 1)       ? (seq_t)(n - 1) * stirling[n - 1][j] : (seq_t)0;
            stirling[n][j] = part1 + part2;
        }
    }

    /* ---------- T table ---------- */
    seq_t **t2 = (seq_t **)realloc(Ttable, (row + 1) * sizeof(seq_t *));
    if (!t2) abort();
    Ttable = t2;
    for (size_t n = built; n <= row; ++n) {
        Ttable[n] = mem_new(seq_t, n + 1);
        if (!Ttable[n]) abort();
    }
    for (size_t n = built; n <= row; ++n) {
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

    built = row + 1;
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
