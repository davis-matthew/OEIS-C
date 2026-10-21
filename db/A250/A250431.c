/*
 * A250431  Number of (n+1)X(7+1) 0..1 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 64, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250431 – number of (n+1)×8 binary arrays with
 * nondecreasing sums of adjacent values in every row and column.
 * Implemented by dynamic programming over the 25 horizontal‑valid
 * 8‑bit rows.  The function a(n) returns the nth term (n ≥ 1). */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc, free, calloc, realloc */
#include <string.h>   /* for memcpy */

#define MAX_ROW_BITS 8
#define MAX_TOT_ROWS 25   /* number of horizontal‑valid rows */

/* ------------------------------------------------------------------- */
/* Pre‑computed tables of horizontal‑valid rows and their transitions   */
/* ------------------------------------------------------------------- */
static unsigned char valid_rows[MAX_TOT_ROWS];                     /* 8‑bit rows */
static unsigned char row_bits[MAX_TOT_ROWS][MAX_ROW_BITS];         /* bits of each row */
static unsigned char sum_vec[MAX_TOT_ROWS][MAX_TOT_ROWS][MAX_ROW_BITS]; /* row sums */
static unsigned char trans_ok[MAX_TOT_ROWS][MAX_TOT_ROWS][MAX_TOT_ROWS];/* transition flag */
static bool rows_built = false;

/* Build the 25 horizontal‑valid rows and pre‑compute transitions */
static void build_rows_and_transitions(void)
{
    int idx = 0;
    for (int row = 0; row < (1 << MAX_ROW_BITS); ++row) {
        unsigned char bits[MAX_ROW_BITS];
        for (int j = 0; j < MAX_ROW_BITS; ++j)
            bits[j] = (row >> j) & 1u;

        bool ok = true;
        for (int j = 1; j < MAX_ROW_BITS; ++j) {
            if (bits[j] + bits[j - 1] < bits[j - 1] + bits[j - 2]) { /* wrap? but not needed */
                /* The horizontal condition is (bit_j + bit_{j+1}) nondecreasing
                 * along columns; we simply check monotonicity of sums of adjacent pairs. */
                ok = false;
                break;
            }
        }
        /* Correct horizontal check: sums are nondecreasing left‑to‑right */
        if (ok) {
            for (int j = 0; j < MAX_ROW_BITS - 1; ++j) {
                unsigned char s = bits[j] + bits[j + 1];
                if (j > 0) {
                    unsigned char prev = bits[j - 1] + bits[j];
                    if (s < prev) {
                        ok = false;
                        break;
                    }
                }
            }
        }
        if (!ok) continue;

        valid_rows[idx] = (unsigned char)row;
        memcpy(row_bits[idx], bits, MAX_ROW_BITS);
        ++idx;
    }

    /* Pre‑compute pair sums for each pair of rows */
    for (size_t a = 0; a < MAX_TOT_ROWS; ++a) {
        for (size_t b = 0; b < MAX_TOT_ROWS; ++b) {
            for (size_t j = 0; j < MAX_ROW_BITS; ++j)
                sum_vec[a][b][j] = row_bits[a][j] + row_bits[b][j];
        }
    }

    /* Pre‑compute transition feasibility a→b→c */
    for (size_t a = 0; a < MAX_TOT_ROWS; ++a) {
        for (size_t b = 0; b < MAX_TOT_ROWS; ++b) {
            for (size_t c = 0; c < MAX_TOT_ROWS; ++c) {
                bool ok = true;
                for (size_t j = 0; j < MAX_ROW_BITS; ++j) {
                    if (sum_vec[a][b][j] > sum_vec[b][c][j]) {
                        ok = false;
                        break;
                    }
                }
                trans_ok[a][b][c] = ok ? 1 : 0;
            }
        }
    }
    rows_built = true;
}

/* ------------------------------------------------------------------- */
/* Main sequence function                                               */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value for offset 1 */

    /* Build tables once */
    if (!rows_built) build_rows_and_transitions();

    /* ----------  Memoization cache  -------------------------------- */
    static seq_t *cache = NULL;          /* index 1 … lastComputedN */
    static size_t cacheCap = 0;
    static size_t lastComputedN = 0;

    /* Expand cache if needed */
    if ((size_t)n > cacheCap) {
        size_t newCap = (size_t)n;
        seq_t *newCache;
#ifdef __cplusplus
        newCache = new seq_t[newCap + 1];
#else
        newCache = (seq_t *)calloc(newCap + 1, sizeof(seq_t));
#endif
        if (cache) {
#ifdef __cplusplus
            for (size_t i = 1; i <= cacheCap; ++i) newCache[i] = cache[i];
            delete[] cache;
#else
            for (size_t i = 1; i <= cacheCap; ++i) newCache[i] = cache[i];
            free(cache);
#endif
        }
        cache = newCache;
        cacheCap = newCap;
    }

    /* ----------  DP tables  ---------------------------------------- */
    static seq_t dpPrev[MAX_TOT_ROWS][MAX_TOT_ROWS];
    static seq_t dpNext[MAX_TOT_ROWS][MAX_TOT_ROWS];
    static size_t currentLen = 0;   /* number of rows represented by dpPrev (≥2) */

    /* Initialize for n = 1 (two rows) if not yet done */
    if (currentLen == 0) {
        for (size_t a = 0; a < MAX_TOT_ROWS; ++a)
            for (size_t b = 0; b < MAX_TOT_ROWS; ++b)
                dpPrev[a][b] = (seq_t)1;

        /* total for n = 1 */
        seq_t total = (seq_t)0;
        for (size_t a = 0; a < MAX_TOT_ROWS; ++a)
            for (size_t b = 0; b < MAX_TOT_ROWS; ++b)
                total += dpPrev[a][b];

        cache[1] = total;
        lastComputedN = 1;
        currentLen = 2;   /* dpPrev represents sequences of length 2 rows (n=1) */
    }

    /* Extend DP until we have computed up to requested n */
    while (lastComputedN < (size_t)n) {
        /* dpNext will hold sequences of length currentLen+1 rows (n = currentLen) */
        for (size_t b = 0; b < MAX_TOT_ROWS; ++b)
            for (size_t c = 0; c < MAX_TOT_ROWS; ++c)
                dpNext[b][c] = (seq_t)0;

        for (size_t b = 0; b < MAX_TOT_ROWS; ++b) {
            for (size_t c = 0; c < MAX_TOT_ROWS; ++c) {
                seq_t acc = (seq_t)0;
                for (size_t a = 0; a < MAX_TOT_ROWS; ++a) {
                    if (trans_ok[a][b][c]) acc += dpPrev[a][b];
                }
                dpNext[b][c] = acc;
            }
        }

        /* total for n = currentLen */
        seq_t total = (seq_t)0;
        for (size_t b = 0; b < MAX_TOT_ROWS; ++b)
            for (size_t c = 0; c < MAX_TOT_ROWS; ++c)
                total += dpNext[b][c];

        cache[currentLen] = total;   /* n = currentLen */
        lastComputedN = currentLen;

        /* Prepare for next iteration */
        for (size_t b = 0; b < MAX_TOT_ROWS; ++b)
            for (size_t c = 0; c < MAX_TOT_ROWS; ++c)
                dpPrev[b][c] = dpNext[b][c];

        ++currentLen;                /* dpPrev now represents sequences of length currentLen rows */
    }

    return cache[(size_t)n];
}
