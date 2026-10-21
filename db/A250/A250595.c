/*
 * A250595  Number of (n+2)X(2+2) 0..2 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * ∏_{r=0}^{2} C(L_r+2,2)^2 * ∏_{r=0}^{2} F(L_r) * * where
 *            L_r = ⌈(n+2−r)/3⌉ is the number of rows with index ≡ r (mod 3),
 *            * and F(k) is the number of pairs of non‑decreasing 0..2
 *            sequences of * length k satisfying u[i] ≤ v[i] for all i. F(k)
 *            is computed by a * simple dynamic program over t...
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 23, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define STATE_COUNT 6

static const uint8_t U[STATE_COUNT] = {0,0,0,1,1,2};
static const uint8_t V[STATE_COUNT] = {0,1,2,1,2,2};
static bool TRANS[STATE_COUNT][STATE_COUNT];
static bool _trans_inited = false;

static void init_trans(void)
{
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            TRANS[i][j] = (U[j] >= U[i]) && (V[j] >= V[i]) && (U[j] <= V[j]);
    _trans_inited = true;
}

/* Compute F(k) for all k ≤ max.  The array must have size max+1. */
static void compute_pair_counts(idx_t max, seq_t *pair_count)
{
    pair_count[0] = (seq_t)1;
    if (max == 0) return;
    pair_count[1] = (seq_t)6;          /* (0,0) (0,1) (0,2) (1,1) (1,2) (2,2) */
    if (max == 1) return;

    seq_t cur[STATE_COUNT], nxt[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = (seq_t)1;

    for (idx_t len = 2; len <= max; ++len) {
        for (int j = 0; j < STATE_COUNT; ++j) nxt[j] = (seq_t)0;
        for (int i = 0; i < STATE_COUNT; ++i) {
            if (cur[i] == (seq_t)0) continue;
            for (int j = 0; j < STATE_COUNT; ++j)
                if (TRANS[i][j]) nxt[j] += cur[i];
        }
        seq_t sum = (seq_t)0;
        for (int j = 0; j < STATE_COUNT; ++j) sum += nxt[j];
        pair_count[(size_t)len] = sum;
        for (int j = 0; j < STATE_COUNT; ++j) cur[j] = nxt[j];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    if (!_trans_inited) init_trans();

    /* Number of rows in the array */
    idx_t m = n + (idx_t)2;           /* n+2 rows, 4 columns */

    /* Count rows in each residue class modulo 3 */
    idx_t L[3];
    idx_t q = m / 3;
    idx_t r = m % 3;
    L[0] = q + ((r >= 1) ? 1 : 0);
    L[1] = q + ((r >= 2) ? 1 : 0);
    L[2] = q;

    /* Determine maximum length needed for the DP (ceil(m/3)) */
    idx_t maxLen = (m + 2) / 3;     /* integer division gives the ceil */

    /* Allocate array for F(k) values */
    seq_t *pair_count = (seq_t*)malloc((size_t)(maxLen + 1) * sizeof(seq_t));
    if (!pair_count) return (seq_t)0;   /* allocation failure */

    compute_pair_counts(maxLen, pair_count);

    /* Compute S = ∏_r binom(L_r+2,2) */
    seq_t S = (seq_t)1;
    for (int r0 = 0; r0 < 3; ++r0) {
        seq_t len = (seq_t)L[r0];
        seq_t term = ((len + (seq_t)2) * (len + (seq_t)1)) / (seq_t)2;
        S *= term;
    }

    /* Compute Φ = ∏_r pair_count[L_r] */
    seq_t phi = (seq_t)1;
    for (int r0 = 0; r0 < 3; ++r0) {
        phi *= pair_count[(size_t)L[r0]];
    }

    seq_t result = phi * S * S;

    free(pair_count);
    return result;
}
