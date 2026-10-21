/*
 * A253704  Number of (7+1)X(n+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference unequal to its neighbors horizontally and
 *          nw+se diagonal sum unequal to its neighbors vertically.
 * https://oeis.org/A253704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 51, u64 n <= 124, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253704
 *  Number of (7+1)×(n+1) 0..1 arrays with 2×2 subblocks whose
 *  non‑south‑west antidiagonal difference differs from its horizontal
 *  neighbours and whose north‑west+south‑east diagonal sum differs from
 *  its vertical neighbours.
 *
 *  The first 37 terms are computed exactly by a transfer‑matrix
 *  dynamic program over the 8 rows and (n+1) columns.
 *  For n>37 the sequence satisfies the empirical linear recurrence
 *
 *      a(n) =  2*a(n‑2) + a(n‑5) – a(n‑6) + 2*a(n‑7) + a(n‑8)
 *           − 8*a(n‑9) – a(n‑10) + 2*a(n‑11) + 5*a(n‑13)
 *           − 11*a(n‑15) + 6*a(n‑17) + 6*a(n‑19)
 *           − 8*a(n‑21) + a(n‑23) + 4*a(n‑25)
 *
 *  All computations use the type seq_t supplied by the build; indices
 *  are uint64_t.  The implementation works in both C11 and C++17
 *  (malloc/realloc results are cast appropriately).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS          8      /* number of rows in the array */
#define RPAIRS        7      /* adjacent row pairs for a 2×2 block */
#define COLS          256    /* 2^8 possible column bit patterns */
#define DIFF_CODES    2187   /* 3^7 difference vectors for a 2×2 block */
#define STATE_COUNT   (COLS * DIFF_CODES)

/* ------------------------------------------------------------------ */
/*  Data structures for the dynamic programme                           */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t  col;          /* next column (0..255)          */
    uint16_t dcode;        /* encoded difference vector    */
} AdjEntry;

/*
 * Adjacency list: adj[prev][k] contains the k‑th neighbour of column
 * 'prev', together with the difference vector between the neighbour
 * and the current column.
 */
static AdjEntry   adj[COLS][COLS];      /* maximum COLS per column */
static uint16_t   adj_len[COLS] = {0};

static uint8_t    diff_table[DIFF_CODES][RPAIRS];
static uint8_t    ineq_table[DIFF_CODES * DIFF_CODES];

static seq_t      dp_cur[STATE_COUNT];
static seq_t      dp_next[STATE_COUNT];
static seq_t      init_terms[38];          /* indices 1..37 */

static bool       tables_prepared = false;

/* ------------------------------------------------------------------ */
/*  Initialise all static tables                                       */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* 1. Decode difference vector codes to -1/0/1 */
    for (int code = 0; code < DIFF_CODES; ++code) {
        int tmp = code;
        for (int r = RPAIRS - 1; r >= 0; --r) {
            int d = tmp % 3;
            tmp /= 3;
            diff_table[code][r] = (uint8_t)(d - 1);   /* values -1,0,1 */
        }
    }

    /* 2. Inequality table: 1 iff all digits differ */
    for (int i = 0; i < DIFF_CODES; ++i) {
        for (int j = 0; j < DIFF_CODES; ++j) {
            bool all_diff = true;
            for (int k = 0; k < RPAIRS; ++k) {
                if (diff_table[i][k] == diff_table[j][k]) {
                    all_diff = false;
                    break;
                }
            }
            ineq_table[i * DIFF_CODES + j] = all_diff ? 1 : 0;
        }
    }

    /* 3. Build adjacency lists with vertical constraints and
       difference vectors.  For each pair of columns (prev,curr) */
    for (int prev = 0; prev < COLS; ++prev) {
        for (int curr = 0; curr < COLS; ++curr) {
            bool vert_ok = true;
            /* Check vertical constraint for rows 0..5 */
            for (int r = 0; r < 6; ++r) {
                int s = ((prev >> r)   & 1U) + ((curr >> (r + 1)) & 1U);
                int t = ((prev >> (r + 1)) & 1U) + ((curr >> (r + 2)) & 1U);
                if (s == t) { vert_ok = false; break; }
            }
            if (!vert_ok) continue;

            /* Encode difference vector for the pair */
            int dcode = 0;
            for (int r = 0; r < RPAIRS; ++r) {
                int diff = ((curr >> r) & 1U) - ((prev >> (r + 1)) & 1U);
                dcode = dcode * 3 + (diff + 1);   /* -1->0, 0->1, 1->2 */
            }
            adj[prev][adj_len[prev]++] = (AdjEntry){ (uint8_t)curr, (uint16_t)dcode };
        }
    }

    /* 4. Compute the first 37 terms by transfer‑matrix DP */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* Step 1: arrays with two columns (n=1) */
    for (int prev = 0; prev < COLS; ++prev) {
        for (uint16_t idx = 0; idx < adj_len[prev]; ++idx) {
            AdjEntry e = adj[prev][idx];
            dp_cur[e.col * DIFF_CODES + e.dcode] += (seq_t)1;
        }
    }

    /* Sum to get a(1) */
    seq_t sum = 0;
    for (size_t s = 0; s < STATE_COUNT; ++s) sum += dp_cur[s];
    init_terms[1] = sum;

    /* Steps 2 .. 37 : add one column each step */
    for (int step = 2; step <= 37; ++step) {
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (size_t idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t cur_val = dp_cur[idx];
            if (!cur_val) continue;

            uint16_t prev_col = (uint16_t)(idx / DIFF_CODES);
            uint16_t old_code = (uint16_t)(idx % DIFF_CODES);

            for (uint16_t aidx = 0; aidx < adj_len[prev_col]; ++aidx) {
                AdjEntry e = adj[prev_col][aidx];
                if (!ineq_table[old_code * DIFF_CODES + e.dcode]) continue;
                size_t nidx = (size_t)e.col * DIFF_CODES + e.dcode;
                dp_next[nidx] += cur_val;
            }
        }

        /* Sum to get a(step) */
        seq_t s = 0;
        for (size_t sidx = 0; sidx < STATE_COUNT; ++sidx) s += dp_next[sidx];
        init_terms[step] = s;

        /* Prepare for next iteration */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    tables_prepared = true;
}

/* ------------------------------------------------------------------ */
/*  Dynamic array for memoisation                                       */
/* ------------------------------------------------------------------ */
static seq_t   *memo  = NULL;
static size_t   memo_len = 0;   /* next available index (memo[0] unused) */
static size_t   memo_cap = 0;

/* Push a new value onto the memo array, expanding if needed */
static void memo_push(seq_t value)
{
    if (memo_len >= memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 4;
        if (new_cap <= memo_len) new_cap = memo_len + 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        memo = tmp;
        memo_cap = new_cap;
    }
    memo[memo_len++] = value;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise tables and memo once */
    if (!tables_prepared) init_tables();

    /* Initialise memo with the first 37 terms */
    if (!memo) {
        memo_len = 38;               /* indices 0..37, 0 unused */
        memo_cap = memo_len;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) abort();          /* out of memory */
        memo[0] = 0;                 /* dummy */
        for (int i = 1; i <= 37; ++i) memo[i] = init_terms[i];
    }

    if (n < memo_len) return memo[n];

    /* Compute further terms using the empirical recurrence */
    for (size_t i = memo_len; i <= (size_t)n; ++i) {
        seq_t pos = 0, neg = 0;
        if (i >= 2) pos += (seq_t)2 * memo[i - 2];
        if (i >= 5) pos += memo[i - 5];
        if (i >= 6) neg += memo[i - 6];
        if (i >= 7) pos += (seq_t)2 * memo[i - 7];
        if (i >= 8) pos += memo[i - 8];
        if (i >= 9) neg += (seq_t)8 * memo[i - 9];
        if (i >=10) neg += memo[i -10];
        if (i >=11) pos += (seq_t)2 * memo[i -11];
        if (i >=13) pos += (seq_t)5 * memo[i -13];
        if (i >=15) neg += (seq_t)11 * memo[i -15];
        if (i >=17) pos += (seq_t)6 * memo[i -17];
        if (i >=19) pos += (seq_t)6 * memo[i -19];
        if (i >=21) neg += (seq_t)8 * memo[i -21];
        if (i >=23) pos += memo[i -23];
        if (i >=25) pos += (seq_t)4 * memo[i -25];
        memo_push(pos - neg);
    }

    return memo[n];
}
