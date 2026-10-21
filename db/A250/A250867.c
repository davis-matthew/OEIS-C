/*
 * A250867  Number of (n+2)X(7+2) 0..3 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 13, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250867: Number of (n+2)x(9) 0..3 arrays with nondecreasing sums
 * over every consecutive triple in rows and columns.
 *
 * The value is computed as
 *     a(n) = ( c(L0) * c(L1) * c(L2) )^3
 * where Li are the split counts of the n+2 rows into three groups
 * (ceil, middle, floor division by 3), and c(L) is the number of
 * chains of length L in the poset of sorted triples (x,y,z) with
 * 0 ≤ x ≤ y ≤ z ≤ 3.
 * The chain counts are pre‑computed once and reused for all calls.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Pre‑computed sorted triples (x,y,z) with 0≤x≤y≤z≤3. */
#define MAX_TRIPLES 20
static const idx_t TRIPLES[MAX_TRIPLES][3] = {
    {0,0,0},{0,0,1},{0,0,2},{0,0,3},
    {0,1,1},{0,1,2},{0,1,3},
    {0,2,2},{0,2,3},
    {0,3,3},
    {1,1,1},{1,1,2},{1,1,3},
    {1,2,2},{1,2,3},
    {1,3,3},
    {2,2,2},{2,2,3},
    {2,3,3},
    {3,3,3}
};

/* Pre‑computed predecessor lists for each triple:
 * pred[i][k] = j  iff  TRIPLES[j] ≤ TRIPLES[i] componentwise.
 * pred_count[i] = number of predecessors of triple i. */
#define MAX_PRED 20
static idx_t pred[MAX_TRIPLES][MAX_PRED];
static unsigned pred_count[MAX_TRIPLES];

/* Flag to compute predecessor lists only once. */
static bool pred_precomputed = false;
static void precompute_predecessors(void) {
    if (pred_precomputed) return;
    pred_precomputed = true;
    for (int i = 0; i < MAX_TRIPLES; ++i) {
        pred_count[i] = 0;
        for (int j = 0; j < MAX_TRIPLES; ++j) {
            if (TRIPLES[j][0] <= TRIPLES[i][0] &&
                TRIPLES[j][1] <= TRIPLES[i][1] &&
                TRIPLES[j][2] <= TRIPLES[i][2]) {
                pred[i][pred_count[i]++] = j;
            }
        }
    }
}

/* Storage for chain counts: c[L] = number of chains of length L. */
static seq_t *chain_counts = NULL;   /* 1‑based indexing: chain_counts[1] used. */
static idx_t chain_cap = 0;          /* allocated size (number of elements). */

/* DP buffers for current and previous length. */
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;
static idx_t dp_cap = 0;

/* Highest length that has been computed so far. */
static idx_t max_chain_computed = 0;

/* Ensure chain_counts has space for lengths up to maxL (inclusive) and
 * compute any missing chain lengths. */
static void init_chain_counts(idx_t maxL) {
    if (maxL <= max_chain_computed) return;
    /* Allocate or grow chain_counts array. */
    if (chain_cap <= maxL) {
        idx_t new_cap = maxL + 1;   /* include index 0 which is unused */
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) return; /* allocation failure – silently abort */
        if (chain_counts) {
            memcpy(new_arr, chain_counts, chain_cap * sizeof(seq_t));
            free(chain_counts);
        }
        chain_counts = new_arr;
        chain_cap   = new_cap;
    }
    /* Allocate or grow DP buffers. */
    if (dp_cap < MAX_TRIPLES) {
        seq_t *new_prev = (seq_t*)malloc(MAX_TRIPLES * sizeof(seq_t));
        seq_t *new_curr = (seq_t*)malloc(MAX_TRIPLES * sizeof(seq_t));
        if (!new_prev || !new_curr) return;
        if (dp_prev) free(dp_prev);
        if (dp_curr) free(dp_curr);
        dp_prev = new_prev;
        dp_curr = new_curr;
        dp_cap  = MAX_TRIPLES;
    }
    precompute_predecessors();
    /* Compute chain counts from the first missing length. */
    idx_t start = max_chain_computed + 1;
    if (start == 1) {
        /* Base case: length 1 chains – each triple is a chain by itself. */
        for (int i = 0; i < MAX_TRIPLES; ++i) dp_prev[i] = 1;
        seq_t total = 0;
        for (int i = 0; i < MAX_TRIPLES; ++i) total += dp_prev[i];
        chain_counts[1] = total;
        start = 2;
    }
    for (idx_t len = start; len <= maxL; ++len) {
        for (int i = 0; i < MAX_TRIPLES; ++i) {
            seq_t s = 0;
            for (unsigned k = 0; k < pred_count[i]; ++k) {
                s += dp_prev[pred[i][k]];
            }
            dp_curr[i] = s;
        }
        /* swap dp_prev and dp_curr */
        seq_t *tmp = dp_prev; dp_prev = dp_curr; dp_curr = tmp;
        /* sum over all triples to get chain_counts[len] */
        seq_t s = 0;
        for (int i = 0; i < MAX_TRIPLES; ++i) s += dp_prev[i];
        chain_counts[len] = s;
    }
    max_chain_computed = maxL;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Number of rows = n+2. */
    idx_t m = n + 2;
    /* Split m into three parts: ceil, middle, floor division by 3. */
    idx_t base = m / 3;
    idx_t rem  = m % 3;
    idx_t L0 = base + (rem >= 1); /* ceil part */
    idx_t L1 = base + (rem >= 2); /* middle part */
    idx_t L2 = base;              /* floor part */

    /* Ensure chain counts up to the largest required length. */
    idx_t maxNeeded = L0;
    if (L1 > maxNeeded) maxNeeded = L1;
    if (L2 > maxNeeded) maxNeeded = L2;
    init_chain_counts(maxNeeded);

    /* Retrieve chain counts. */
    seq_t c0 = chain_counts[L0];
    seq_t c1 = chain_counts[L1];
    seq_t c2 = chain_counts[L2];

    /* Compute (c0 * c1 * c2)^3. */
    seq_t prod = c0;
    prod *= c1;
    prod *= c2;
    seq_t ans = prod;
    ans *= prod;
    ans *= prod;
    return ans;
}
