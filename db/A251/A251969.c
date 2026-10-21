/*
 * A251969  Number of (n+1)X(2+1) 0..3 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A251969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 44, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251969: Number of (n+1)×(2+1) 0..3 arrays with nondecreasing sum of
   every two consecutive values in every row and column.
   The values are counted by the following combinatorial method:

   * A row is a triple (a,b,c) with 0≤a,b,c≤3 and a≤c.
   * For a “column j”, the sums a_{i,j}+a_{i+1,j} must be nondecreasing.
     This is equivalent to a_{i,j} ≤ a_{i+2,j} for all i.
     Consequently the rows with even indices form a nondecreasing chain,
     and rows with odd indices form another independent nondecreasing chain.
   * Thus the total number of arrays of size (n+1)×3 is
        A(n) = C(e)*C(o)
     where e = ⌊n/2⌋+1 is the number of even rows, o = (n+1)-e is the
     number of odd rows, and C(k) is the number of nondecreasing sequences
     of length k over the 40 possible rows.

   C(k) is computed with a simple DP on the partial order:
        dp_len[v]  = number of chains of length 'len' ending at row v
        dp_len[v] = Σ_{u ≤ v} dp_{len-1}[u]
   with dp_1[v] = 1 for all rows v.  The values are stored in an array
   res[len] = Σ_v dp_len[v].
   The array of C(k) grows only once, and can be reused for any order of
   calls. */

/* ------------------------------------------------------------ */
/* ------ Standard includes ----------------------------------- */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* ------ Constants and static data ---------------------------- */

#define NUM_ROWS 40       /* number of possible rows  (a,b,c) with 0≤a,b,c≤3, a≤c */

/* Rows, each row is 3 bytes: [a0, a1, a2] */
static uint8_t rows[NUM_ROWS][3];

/* Predecessor lists for partial order: u ≤ v componentwise. */
static int pred_cnt[NUM_ROWS];
static int pred_list[NUM_ROWS][NUM_ROWS];

/* DP arrays for current and previous length.  Two buffers are enough. */
static seq_t dp_prev[NUM_ROWS];
static seq_t dp_cur[NUM_ROWS];

/* Cache of chain counts: res[k] = C(k) for 1 ≤ k ≤ cap. */
static seq_t *res = NULL;      /* dynamically grown array */
static size_t res_cap = 0;     /* current capacity of res */

/* ------------------------------------------------------------ */
/* ------ Helper: build rows and partial-order graph ------------ */

static void init_rows_and_preds(void)
{
    int idx = 0;
    for (uint8_t a0 = 0; a0 <= 3; ++a0) {
        for (uint8_t a2 = a0; a2 <= 3; ++a2) {
            for (uint8_t a1 = 0; a1 <= 3; ++a1) {
                rows[idx][0] = a0;
                rows[idx][1] = a1;
                rows[idx][2] = a2;
                ++idx;
            }
        }
    }

    /* Should result in NUM_ROWS = 40 */
    (void)idx; /* silence unused variable warning */

    /* Build predecessor lists */
    for (int i = 0; i < NUM_ROWS; ++i)
        pred_cnt[i] = 0;

    for (int u = 0; u < NUM_ROWS; ++u) {
        for (int v = 0; v < NUM_ROWS; ++v) {
            if (rows[u][0] <= rows[v][0] &&
                rows[u][1] <= rows[v][1] &&
                rows[u][2] <= rows[v][2]) {
                pred_list[v][pred_cnt[v]++] = u;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* ------ Ensure chain counts up to 'need' (inclusive) -------- */

static void ensure_counts(size_t need)
{
    /* Build graph once */
    static bool graph_built = false;
    if (!graph_built) {
        init_rows_and_preds();
        graph_built = true;
    }

    if (need <= res_cap)
        return;

    /* Reallocate result cache */
    seq_t *new_res = (seq_t *)realloc(res, (need + 1) * sizeof(seq_t));
    if (!new_res) {
        /* Out of memory – abort the program */
        exit(EXIT_FAILURE);
    }
    res = new_res;

    /* If this is the first time, initialize base case */
    if (res_cap == 0) {
        for (int i = 0; i < NUM_ROWS; ++i)
            dp_prev[i] = (seq_t)1;  /* chains of length 1 */
        res[1] = NUM_ROWS;          /* 40 possible rows */
        res_cap = 1;
    }

    /* Extend DP to required length */
    for (size_t len = res_cap + 1; len <= need; ++len) {
        /* Compute dp_cur for length len */
        for (int v = 0; v < NUM_ROWS; ++v) {
            seq_t s = 0;
            int pc = pred_cnt[v];
            for (int pi = 0; pi < pc; ++pi) {
                int u = pred_list[v][pi];
                s += dp_prev[u];
            }
            dp_cur[v] = s;
        }

        /* Sum over all ending states to get C(len) */
        seq_t total = 0;
        for (int v = 0; v < NUM_ROWS; ++v)
            total += dp_cur[v];
        res[(size_t)len] = total;

        /* Prepare for next iteration */
        for (int v = 0; v < NUM_ROWS; ++v)
            dp_prev[v] = dp_cur[v];
    }

    res_cap = need;
}

/* ------------------------------------------------------------ */
/* ------ Public function -------------------------------------- */

seq_t a(idx_t n)
{
    /* Number of even rows : indices 0,2,4,... ≤ n
       Number of odd  rows : remaining rows  */
    idx_t k_even = n / 2 + 1;              /* floor division works for unsigned */
    idx_t k_odd  = (n + 1) - k_even;

    size_t need = (size_t)k_even;
    if ((size_t)k_odd > need)
        need = (size_t)k_odd;

    ensure_counts(need);

    seq_t even_cnt = res[k_even];
    seq_t odd_cnt  = res[k_odd];

    return even_cnt * odd_cnt;
}
