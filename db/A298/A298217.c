/*
 * A298217  Number of n X 4 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   allowcnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298217:
   Number of n × 4 0..1 arrays with upper‑left element 0,
   each cell equal to 0,1,2,4 or 5 king‑move adjacent elements.
   We compute by dynamic programming over 4‑bit row patterns. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allowcnt.h"

#define WIDTH 4
#define PAT  (1 << WIDTH)          /* 16 patterns */
#define LEFTBIT (1 << (WIDTH - 1)) /* mask for the leftmost column */

/* bit in pattern: column 0 = leftmost */
static inline int bit(int pat, int col) { return (pat >> (WIDTH-1-col)) & 1; }

/* validity tables */
static bool valid_first[PAT][PAT];
static bool valid_row[PAT][PAT][PAT];
static bool valid_last[PAT][PAT];
static bool tables_built = false;

/* pre‑compute all validity tables once */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;
    for (int a = 0; a < PAT; ++a)
    {
        for (int b = 0; b < PAT; ++b)
        {
            /* first row (no upper neighbors) */
            bool ok = true;
            for (int j = 0; j < WIDTH; ++j)
            {
                int val = bit(b,j);
                int same = 0;
                if (j>0 && bit(b,j-1)==val) ++same;
                if (j+1<WIDTH && bit(b,j+1)==val) ++same;
                if (j>0 && bit(a,j-1)==val) ++same;
                if (bit(a,j)==val) ++same;
                if (j+1<WIDTH && bit(a,j+1)==val) ++same;
                if (!allowcnt(same)) { ok = false; break; }
            }
            valid_first[b][a] = ok;   /* first row = b, second = a */

            /* last row (no lower neighbors) */
            ok = true;
            for (int j = 0; j < WIDTH; ++j)
            {
                int val = bit(b,j);
                int same = 0;
                if (j>0 && bit(a,j-1)==val) ++same;
                if (bit(a,j)==val)  ++same;
                if (j+1<WIDTH && bit(a,j+1)==val) ++same;
                if (j>0 && bit(b,j-1)==val) ++same;
                if (j+1<WIDTH && bit(b,j+1)==val) ++same;
                if (!allowcnt(same)) { ok = false; break; }
            }
            valid_last[b][a] = ok;   /* current = b, previous = a */

            /* middle rows */
            for (int c = 0; c < PAT; ++c)
            {
                ok = true;
                for (int j = 0; j < WIDTH; ++j)
                {
                    int val = bit(b,j);
                    int same = 0;
                    if (j>0 && bit(a,j-1)==val) ++same;
                    if (bit(a,j)==val)  ++same;
                    if (j+1<WIDTH && bit(a,j+1)==val) ++same;
                    if (j>0 && bit(b,j-1)==val) ++same;
                    if (j+1<WIDTH && bit(b,j+1)==val) ++same;
                    if (j>0 && bit(c,j-1)==val) ++same;
                    if (bit(c,j)==val)  ++same;
                    if (j+1<WIDTH && bit(c,j+1)==val) ++same;
                    if (!allowcnt(same)) { ok = false; break; }
                }
                valid_row[a][b][c] = ok; /* prev=a, curr=b, next=c */
            }
        }
    }
}

/* DP tables */
static seq_t (*dp_cur)[PAT]  = NULL;
static seq_t (*dp_next)[PAT] = NULL;
static size_t cur_n = 0;              /* number of rows processed in dp_cur */

/* cache for computed sequence values, 1‑based */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t n)
{
    if (n+1 <= cache_cap) return;
    size_t new_cap = (n+1) * 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – keep old cache */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* compute all terms up to target (inclusive) */
static void compute_up_to(size_t target)
{
    build_tables();
    if (cache_len == 0)
    {
        ensure_cache(2);
        seq_cache[1] = (seq_t)8;   /* a(1) */
        cache_len  = 1;
    }

    while (cache_len < target)
    {
        if (cache_len == 1)   /* initialize DP for n = 2 */
        {
            static seq_t tmpA[PAT][PAT];
            static seq_t tmpB[PAT][PAT];
            dp_cur = tmpA;
            dp_next = tmpB;
            for (int i = 0; i < PAT; ++i)
                for (int j = 0; j < PAT; ++j)
                    dp_cur[i][j] = (seq_t)0;
            for (int r0 = 0; r0 < PAT; ++r0)
            {
                if (r0 & LEFTBIT) continue;          /* upper left must be 0 */
                for (int r1 = 0; r1 < PAT; ++r1)
                    if (valid_first[r0][r1])
                        dp_cur[r0][r1] = (seq_t)1;
            }
            cur_n = 2;
            /* compute a(2) */
            seq_t total = (seq_t)0;
            for (int prev = 0; prev < PAT; ++prev)
                for (int curr = 0; curr < PAT; ++curr)
                    if (valid_last[curr][prev]) /* curr is last row */
                        total += dp_cur[prev][curr];
            ensure_cache(2);
            seq_cache[2] = total;
            cache_len = 2;
            continue;
        }

        /* add one more row (increase from cur_n to cur_n+1) */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int prev = 0; prev < PAT; ++prev)
            for (int curr = 0; curr < PAT; ++curr)
            {
                seq_t v = dp_cur[prev][curr];
                if (!v) continue;
                for (int next = 0; next < PAT; ++next)
                    if (valid_row[prev][curr][next])
                        dp_next[curr][next] += v;
            }

        /* swap dp tables */
        seq_t (*tmp)[PAT] = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        ++cur_n;

        /* compute a(cur_n) */
        seq_t total = (seq_t)0;
        for (int prev = 0; prev < PAT; ++prev)
            for (int curr = 0; curr < PAT; ++curr)
                if (valid_last[curr][prev])
                    total += dp_cur[prev][curr];
        ensure_cache(cur_n);
        seq_cache[cur_n] = total;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    compute_up_to((size_t)n);
    return seq_cache[(size_t)n];
}
