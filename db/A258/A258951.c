/*
 * A258951  Number of (n+3)X(2+3) 0..1 arrays with no 4x4 subblock diagonal
 *          sum not equal the antidiagonal sum.
 * https://oeis.org/A258951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 30, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
   Number of (n+3)×5 binary arrays with no 4×4 subblock whose main-diagonal
   sum differs from its anti-diagonal sum.
   The function DP's over the rows, maintaining the last three rows
   as a state.  For each state we precompute all admissible next rows.
   Transitions are stored in a table of size 32768×8 (at most 8 successors
   per state).  The result for a given n is the total number of
   admissible prefixes of length n+3.  The computation is memoised so
   subsequent calls are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Transition tables ----------
   There are 2^15 = 32768 states (three 5‑bit rows).  For each state,
   we store the number of successors and the successor state values.  */
static uint16_t       trans[32768][8];   /* successor state for each transition   */
static uint8_t        trans_cnt[32768];  /* number of transitions for each state */
static bool           trans_done = false;

/* ---------- Pre‑computed bit‑difference tables ----------
   Columns are numbered 0..4 (LSB is column 0).  The values are
   differences between two column bits of a row. */
static signed char diff41[32], /* col3 - col0   */
                   diff32[32], /* col2 - col1   */
                   diff52[32], /* col4 - col1   */
                   diff43[32], /* col3 - col2   */
                   diff34[32]; /* col2 - col3   */

/* Initialise the difference tables. */
static void init_diff(void)
{
    for (int r = 0; r < 32; ++r) {
        int b0 =  r       & 1;
        int b1 = (r >> 1) & 1;
        int b2 = (r >> 2) & 1;
        int b3 = (r >> 3) & 1;
        int b4 = (r >> 4) & 1;

        diff41[r] =  b3 - b0;
        diff32[r] =  b2 - b1;
        diff52[r] =  b4 - b1;
        diff43[r] =  b3 - b2;
        diff34[r] =  b2 - b3;
    }
}

/* Build the transition tables once. */
static void init_trans(void)
{
    if (trans_done) return;
    init_diff();

    for (int s = 0; s < 32768; ++s) {
        /* Extract the three rows from the state */
        int a =  s       & 31;          /* row 1  (bits 0–4) */
        int b = (s >>  5) & 31;          /* row 2  (bits 5–9) */
        int c = (s >> 10) & 31;          /* row 3  (bits10–14) */

        /* Constraints on the next row (row d) to keep all 4×4 blocks
           with equal diagonal sums.  The sums are expressed using the
           pre‑computed differences. */
        int K1 = diff41[a] + diff32[b] - diff32[c];
        int K2 = diff52[a] + diff43[b] + diff34[c];

        if (K1 < -1 || K1 > 1 || K2 < -1 || K2 > 1) {
            trans_cnt[s] = 0;
            continue;
        }

        /* Compute all valid combinations for the new row's bits. */
        int d1_vals[2], d4_vals[2], n1 = 0;
        int d2_vals[2], d5_vals[2], n2 = 0;

        /* d1 (col0) and d4 (col3) are linked by K1 */
        switch (K1) {
            case 0: d1_vals[0] = 0; d4_vals[0] = 0;
                    d1_vals[1] = 1; d4_vals[1] = 1;
                    n1 = 2; break;
            case 1: d1_vals[0] = 0; d4_vals[0] = 1; n1 = 1; break;
            case -1: d1_vals[0] = 1; d4_vals[0] = 0; n1 = 1; break;
        }

        /* d2 (col1) and d5 (col4) are linked by K2 */
        switch (K2) {
            case 0: d2_vals[0] = 0; d5_vals[0] = 0;
                    d2_vals[1] = 1; d5_vals[1] = 1;
                    n2 = 2; break;
            case 1: d2_vals[0] = 0; d5_vals[0] = 1; n2 = 1; break;
            case -1: d2_vals[0] = 1; d5_vals[0] = 0; n2 = 1; break;
        }

        /* Store the successors */
        uint16_t *dst = trans[s];
        for (int i1 = 0; i1 < n1; ++i1) {
            int d1 = d1_vals[i1];
            int d4 = d4_vals[i1];
            for (int i2 = 0; i2 < n2; ++i2) {
                int d2 = d2_vals[i2];
                int d5 = d5_vals[i2];
                for (int d3 = 0; d3 < 2; ++d3) {
                    int row =  d1
                            | (d2 << 1)
                            | (d3 << 2)
                            | (d4 << 3)
                            | (d5 << 4);
                    int nxt =  b | (c << 5) | (row << 10);
                    *dst++ = nxt;
                }
            }
        }
        trans_cnt[s] = dst - trans[s];
    }
    trans_done = true;
}

/* ---------- DP buffers ----------
   Two arrays of counts for the current and next DP layers.  */
static seq_t dp_buf0[32768];
static seq_t dp_buf1[32768];
static seq_t *dp_cur  = dp_buf0;
static seq_t *dp_next = dp_buf1;
static int    dp_width = 3;        /* rows processed so far (initial 3) */
static bool   dp_done  = false;

/* ---------- Cache for computed a(n) ----------
   We keep all results up to the largest n requested. */
static seq_t *cache      = NULL;   /* array of seq_t indexed by n (1‑based) */
static size_t cache_cap  = 0;       /* current cache capacity */

/* Make sure the cache can hold at least (n+1) entries. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;   /* index 0 unused */
    seq_t *old = cache;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    delete[] old;
#else
    new_cache = realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache) {
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    }
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Perform one DP step: add a new row. */
static void dp_step(void)
{
    memset(dp_next, 0, sizeof(dp_buf0));

    for (int s = 0; s < 32768; ++s) {
        seq_t val = dp_cur[s];
        if (val == 0) continue;
        uint8_t  cnt = trans_cnt[s];
        uint16_t *k   = trans[s];
        for (uint8_t i = 0; i < cnt; ++i)
            dp_next[k[i]] += val;
    }

    /* swap buffers and increment the processed row count */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;
    ++dp_width;
}

/* Total number of DP states after current width. */
static seq_t dp_total(void)
{
    seq_t total = 0;
    for (int i = 0; i < 32768; ++i)
        total += dp_cur[i];
    return total;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – a(1) is first term */

    /* Build transitions once. */
    init_trans();

    /* Initialise DP base layer only once. */
    if (!dp_done) {
        for (int i = 0; i < 32768; ++i) dp_cur[i] = 1;   /* all 3‑row prefixes */
        dp_done = true;
    }

    /* Extend DP until we have computed width = n+3 rows. */
    while ((idx_t)(dp_width - 3) < n) {
        dp_step();                     /* add next row */
        ensure_cache((idx_t)(dp_width - 3));
        cache[dp_width - 3] = dp_total();
    }

    /* Ensure cache contains the requested value. */
    ensure_cache(n);
    if (cache[n] == 0) {
        /* Should not happen but keep safe. */
        cache[n] = dp_total();
    }
    return cache[n];
}
