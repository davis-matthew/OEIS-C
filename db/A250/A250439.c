/*
 * A250439  Number of (n+1)X(4+1) 0..2 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (#chains of even‑index rows of length ceil((n+1)/2)) * *
 *            (#chains of odd‑index rows of length floor((n+1)/2)) * Here a
 *            chain is a sequence of rows (each row is a 5‑tuple of 0,1,2) *
 *            such that each row is “good” (its consecutive pair‑sums are
 *            nondecreasing) * and each row is component‑wise...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 33, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Max n expected in the tests (≈10000) */
#define MAX_N 10000
/* The longest chain needed for a 10000‑row array is ceil((MAX_N+1)/2) */
#define MAX_M (MAX_N/2 + 1)

/* Total number of 5‑tuples from {0,1,2} */
#define TOTAL_ROWS (3 * 3 * 3 * 3 * 3)
/* An upper bound on the number of “good” rows (actually 60) */
#define MAX_GOOD TOTAL_ROWS

static bool initialized = false;

/* Data structures used after the first call */
static idx_t G;                                   /* number of good rows           */
static uint8_t good_rows[MAX_GOOD][5];             /* goodRows[i][k]                */
static idx_t *pred_list[MAX_GOOD];                /* list of predecessors of i     */
static size_t pred_cnt[MAX_GOOD];                  /* number of predecessors        */
static seq_t chain_counts[MAX_M + 1];              /* chain_counts[len]             */

/* auxiliary buffers for DP */
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;

/*-------------------------------------------------------------*/
/* Compare two 5‑tuples element‑wise: a <= b ? */
static inline bool
row_leq(const uint8_t *a, const uint8_t *b)
{
    return a[0] <= b[0] && a[1] <= b[1] && a[2] <= b[2]
           && a[3] <= b[3] && a[4] <= b[4];
}

/*--------------------------------------------*/
/* initialise all static structures on first call */
static void
initialize(void)
{
    if (initialized) return;
    initialized = true;

    /* 1. build the list of good rows */
    G = 0;
    for (idx_t num = 0; num < TOTAL_ROWS; ++num) {
        uint8_t val[5];
        idx_t tmp = num;
        for (int k = 4; k >= 0; --k) {
            val[k] = tmp % 3;
            tmp /= 3;
        }
        /* check horizontal monotonicity of sums */
        uint8_t s0 = val[0] + val[1];
        uint8_t s1 = val[1] + val[2];
        uint8_t s2 = val[2] + val[3];
        uint8_t s3 = val[3] + val[4];
        if (s0 <= s1 && s1 <= s2 && s2 <= s3) {
            for (int k = 0; k < 5; ++k) good_rows[G][k] = val[k];
            ++G;
        }
    }

    /* 2. pre‑compute predecessor lists (a <= b) */
    for (idx_t j = 0; j < G; ++j) {
        pred_cnt[j] = 0;
        /* first count how many predecessors to allocate space */
        for (idx_t i = 0; i < G; ++i)
            if (row_leq(good_rows[i], good_rows[j]))
                ++pred_cnt[j];
        pred_list[j] = (idx_t*)malloc(pred_cnt[j] * sizeof(idx_t));
        size_t pos = 0;
        for (idx_t i = 0; i < G; ++i)
            if (row_leq(good_rows[i], good_rows[j]))
                pred_list[j][pos++] = i;
    }

    /* 3. allocate DP arrays */
    dp_prev = (seq_t*)calloc(G, sizeof(seq_t));
    dp_curr = (seq_t*)calloc(G, sizeof(seq_t));
    if (!dp_prev || !dp_curr) abort(); /* out of memory */

    /* 4. compute chain counts for all lengths up to MAX_M */
    /* dp_prev represents chains of current length */
    /* length 1: all chains are single rows */
    for (idx_t j = 0; j < G; ++j)
        dp_prev[j] = (seq_t)1;
    chain_counts[1] = (seq_t)G;

    for (idx_t len = 2; len <= MAX_M; ++len) {
        for (idx_t j = 0; j < G; ++j) {
            seq_t sum = 0;
            for (size_t p = 0; p < pred_cnt[j]; ++p)
                sum += dp_prev[pred_list[j][p]];
            dp_curr[j] = sum;
        }
        seq_t tot = 0;
        for (idx_t j = 0; j < G; ++j)
            tot += dp_curr[j];
        chain_counts[len] = tot;

        /* swap buffers for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }
}

/*-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (!initialized) initialize();

    /* lengths of even‑index and odd‑index chains */
    idx_t evenLen = n / 2 + 1;             /* ceil((n+1)/2) for n>=1 */
    idx_t oddLen  = (n + 1) / 2;            /* floor((n+1)/2)        */

    if (evenLen > MAX_M || oddLen > MAX_M)
        return 0; /* out of pre‑computed range */

    seq_t res = chain_counts[evenLen] * chain_counts[oddLen];
    return res;
}
