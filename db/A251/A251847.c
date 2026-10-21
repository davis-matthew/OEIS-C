/*
 * A251847  Number of (n+2)X(2+2) 0..3 arrays with every row, column, diagonal
 *          or antidiagonal in each 3X3 subblock summing to a prime.
 * https://oeis.org/A251847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A251847.
   a(n) counts the number of (n+2)×4 arrays with entries 0..3 such that
   every 3×3 subblock (rows i..i+2, columns 0..2 or 1..3) has all row,
   column, main‑diagonal and anti‑diagonal sums prime (2,3,5,7). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATE_COUNT 65536U   /* 256 × 256 possible 2‑row states */

static bool prime[10];
static uint8_t row_digits[256][4];
static int row_row_sum[256][2];

typedef struct {
    int  *data;   /* next states */
    size_t len;
    size_t cap;
} EdgeList;

static EdgeList *adjacency;
static bool      precomputed = false;

/* Test whether a triple of rows (a,b,c) satisfies all 3×3 subblock constraints. */
static inline bool triple_valid(int a, int b, int c)
{
    /* Row sums of the triple in the two possible 3‑column windows */
    if (!prime[row_row_sum[a][0]] || !prime[row_row_sum[a][1]] ||
        !prime[row_row_sum[b][0]] || !prime[row_row_sum[b][1]] ||
        !prime[row_row_sum[c][0]] || !prime[row_row_sum[c][1]]) {
        return false;
    }

    /* Column sums for all four columns (columns 0–3 of the 4‑wide array) */
    if (!prime[row_digits[a][0] + row_digits[b][0] + row_digits[c][0]]) return false;
    if (!prime[row_digits[a][1] + row_digits[b][1] + row_digits[c][1]]) return false;
    if (!prime[row_digits[a][2] + row_digits[b][2] + row_digits[c][2]]) return false;
    if (!prime[row_digits[a][3] + row_digits[b][3] + row_digits[c][3]]) return false;

    /* Main diagonals of the 3×3 subblocks */
    if (!prime[row_digits[a][0] + row_digits[b][1] + row_digits[c][2]]) return false;
    if (!prime[row_digits[a][1] + row_digits[b][2] + row_digits[c][3]]) return false;

    /* Anti‑diagonals of the 3×3 subblocks */
    if (!prime[row_digits[a][2] + row_digits[b][1] + row_digits[c][0]]) return false;
    if (!prime[row_digits[a][3] + row_digits[b][2] + row_digits[c][1]]) return false;

    return true;
}

/* Build the adjacency lists for all 2‑row states. */
static void precompute(void)
{
    /* Primes up to 9 */
    for (int i = 0; i < 10; ++i) prime[i] = false;
    prime[2] = prime[3] = prime[5] = prime[7] = true;

    /* Precompute the digits of every 4‑cell row and their row sums. */
    for (int r = 0; r < 256; ++r) {
        row_digits[r][0] = (r >> 0) & 3;
        row_digits[r][1] = (r >> 2) & 3;
        row_digits[r][2] = (r >> 4) & 3;
        row_digits[r][3] = (r >> 6) & 3;
        row_row_sum[r][0] = row_digits[r][0] + row_digits[r][1] + row_digits[r][2];
        row_row_sum[r][1] = row_digits[r][1] + row_digits[r][2] + row_digits[r][3];
    }

    adjacency = (EdgeList *)calloc(STATE_COUNT, sizeof(EdgeList));
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            EdgeList *e = &adjacency[(a << 8) | b];
            for (int c = 0; c < 256; ++c) {
                if (triple_valid(a, b, c)) {
                    if (e->len == e->cap) {
                        size_t newcap = (e->cap ? e->cap << 1 : 4);
                        int *tmp = (int *)realloc(e->data, newcap * sizeof(int));
                        if (!tmp) abort();
                        e->data = tmp;
                        e->cap = newcap;
                    }
                    e->data[e->len++] = (b << 8) | c;
                }
            }
        }
    }
    precomputed = true;
}

seq_t a(idx_t n)
{
    if (!precomputed) precompute();

    const size_t S = STATE_COUNT;

    seq_t *dpA = (seq_t *)calloc(S, sizeof(seq_t));
    seq_t *dpB = (seq_t *)calloc(S, sizeof(seq_t));
    if (!dpA || !dpB) abort();

    int *actA = (int *)calloc(S, sizeof(int));
    int *actB = (int *)calloc(S, sizeof(int));
    if (!actA || !actB) abort();

    size_t actA_len = 0, actB_len = 0;

    /* Initially all 2‑row states are possible. */
    for (size_t i = 0; i < S; ++i) {
        dpA[i]      = (seq_t)1;
        actA[actA_len++] = (int)i;
    }

    /* Perform n transitions (one for each new row beyond the first two). */
    for (idx_t step = 0; step < n; ++step) {
        actB_len = 0;
        for (size_t ia = 0; ia < actA_len; ++ia) {
            int          idx = actA[ia];
            seq_t        val = dpA[idx];
            EdgeList    *e   = &adjacency[idx];
            for (size_t j = 0; j < e->len; ++j) {
                int ns = e->data[j];
                if (dpB[ns] == 0) {
                    actB[actB_len++] = ns;
                }
                dpB[ns] += val;
            }
        }

        /* Clear old dpA entries for reuse. */
        for (size_t ia = 0; ia < actA_len; ++ia) {
            dpA[actA[ia]] = (seq_t)0;
        }

        /* Swap dp arrays and active lists. */
        seq_t  *tmpdp  = dpA; dpA = dpB; dpB = tmpdp;
        int    *tmpact = actA; actA = actB; actB = tmpact;
        actA_len = actB_len;
    }

    /* Sum all counts in the final state set. */
    seq_t total = (seq_t)0;
    for (size_t ia = 0; ia < actA_len; ++ia) {
        total += dpA[actA[ia]];
    }

    free(dpA);
    free(dpB);
    free(actA);
    free(actB);

    return total;
}
