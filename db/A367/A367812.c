/*
 * A367812  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that the Levenshtein distance (Ld) between a(n) and a(n+1) is
 *          equal to 2.
 * https://oeis.org/A367812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   ensure_vals_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ensure_vals_capacity.h"

/* Computes A367812 (lexicographically earliest sequence of distinct
   nonnegative integers such that the Levenshtein distance between
   consecutive terms is exactly 2). */

#define MAX_DIGITS 40

/* ------------------------------------------------------------------ */
/* Utility: extract decimal digits (most significant first )          */
/* ------------------------------------------------------------------ */
static void get_digits(seq_t x, unsigned char buf[MAX_DIGITS], int *len)
{
    if (x == (seq_t)0) {
        buf[0] = 0;
        *len  = 1;
        return;
    }
    int pos = 0;
    seq_t t = x;
    while (t != (seq_t)0) {
        buf[pos++] = (unsigned char)(t % (seq_t)10);
        t = t / (seq_t)10;
    }
    /* reverse the buffer into a temporary array */
    unsigned char rev[MAX_DIGITS];
    for (int i = 0; i < pos; ++i)
        rev[i] = buf[pos - 1 - i];
    for (int i = 0; i < pos; ++i)
        buf[i] = rev[i];
    *len = pos;
}

/* ------------------------------------------------------------------ */
/* Levenshtein distance between two digit arrays                       */
/* ------------------------------------------------------------------ */
static int levenshtein_dist2(const unsigned char *a, int la,
                             const unsigned char *b, int lb)
{
    static int dp[MAX_DIGITS][MAX_DIGITS];
    for (int i = 0; i <= la; ++i) dp[i][0] = i;
    for (int j = 0; j <= lb; ++j) dp[0][j] = j;
    for (int i = 1; i <= la; ++i) {
        for (int j = 1; j <= lb; ++j) {
            int sub = dp[i-1][j-1] + (a[i-1] != b[j-1] ? 1 : 0);
            int del = dp[i-1][j] + 1;
            int ins = dp[i][j-1] + 1;
            int m = sub;
            if (del < m) m = del;
            if (ins < m) m = ins;
            dp[i][j] = m;
        }
    }
    return dp[la][lb];
}

/* ------------------------------------------------------------------ */
/* Helper: ensure capacity of a bool array for “used” values           */
/* ------------------------------------------------------------------ */
static void ensure_used_capacity(idx_t val, bool **used, idx_t *cap)
{
    if (val < *cap) return;
    idx_t newcap = (*cap) ? (*cap) * 2 : 2;
    while (newcap <= val) newcap <<= 1;
    bool *tmp = (bool *)realloc(*used, newcap * sizeof(**used));
    if (!tmp) exit(EXIT_FAILURE);
    for (idx_t i = *cap; i < newcap; ++i) tmp[i] = false;
    *used = tmp;
    *cap  = newcap;
}

/* ------------------------------------------------------------------ */
/* Helper: ensure capacity of a seq_t array for cached values          */

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */

    static seq_t *vals = NULL;
    static idx_t vals_cnt = 0, vals_cap = 0;
    static bool *used = NULL;
    static idx_t used_cap = 0;

    /* Initialise first term */
    if (vals_cnt == 0) {
        ensure_vals_capacity(1, &vals, &vals_cap);
        vals[0] = (seq_t)0;
        vals_cnt = 1;
        ensure_used_capacity(0, &used, &used_cap);
        used[0] = true;
    }

    /* Build the sequence up to n if necessary */
    while (vals_cnt < n) {
        seq_t prev = vals[vals_cnt - 1];
        unsigned char prev_digs[MAX_DIGITS], cand_digs[MAX_DIGITS];
        int prev_len, cand_len;

        get_digits(prev, prev_digs, &prev_len);

        idx_t cand_idx = 0;
        while (true) {
            /* Candidate value */
            seq_t cand_value = (seq_t)cand_idx;

            /* Skip used values */
            if (cand_idx < used_cap && used[cand_idx]) {
                ++cand_idx;
                continue;
            }

            ensure_used_capacity(cand_idx, &used, &used_cap);
            get_digits(cand_value, cand_digs, &cand_len);

            int d = levenshtein_dist2(prev_digs, prev_len,
                                       cand_digs, cand_len);
            if (d == 2) {
                /* Accept candidate */
                ensure_vals_capacity(vals_cnt + 1, &vals, &vals_cap);
                vals[vals_cnt] = cand_value;
                ++vals_cnt;
                used[cand_idx] = true;
                break;
            }
            ++cand_idx;
        }
    }

    return vals[n - 1];
}
