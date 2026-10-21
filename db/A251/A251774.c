/*
 * A251774  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 3 or 6 and every diagonal and antidiagonal sum not 3 or
 *          6.
 * https://oeis.org/A251774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   transition_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the number of (n+2)×5 arrays using digits 0..3
 * such that every 3×3 subblock has each row and column sum equal to
 * 3 or 6, and every main and anti‑diagonal of every 3×3 subblock
 * has sum different from 3 and 6.
 *
 * This implementation counts the arrays by dynamic programming
 * over the last two rows of the array.  It pre‑computes all 5‑digit
 * rows that satisfy the row/column constraints, the admissible
 * transitions between consecutive triples of rows, and then
 * iteratively builds the counts for heights 3,4,…,n+2.
 *
 * The function is memoised; it will automatically handle calls
 * for any n≥1 in any order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/transition_ok.h"

/* --- constants ------------------------------------------------------- */
#define DIGITS   5   /* width of the rows (3+2) */
#define MAX_MASK (1 << (2 * DIGITS))  /* 2 bits per digit */

/* --- global tables (initialised by init_once) ----------------------- */
static bool     initialized = false;

/* number of admissible rows */
static size_t M = 0;
/* each row: 5 digits 0..3 */
static int      row_digits[1024][DIGITS];   /* 1024 > maximum possible */

/* transition data: for each ordered pair (p,q) the list of admissible r */
static size_t  *trans_start = NULL;   /* start offset in flat array */
static size_t  *trans_len   = NULL;   /* number of successors      */
static int     *trans_next  = NULL;   /* flat array of successors   */

/* DP tables: dp[i*M + j] = number of arrays of current height ending
 * with rows i (second last) and j (last).  newdp is used for the next height. */
static seq_t   *dp      = NULL;
static seq_t   *newdp   = NULL;
static size_t  pairCount = 0;

/* memoisation cache for a(n) */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static idx_t  cached_n  = 0;

/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* initialise all tables once ------------------------------------------------ */
static void init_once(void)
{
    /* 1) Build admissible rows */
    for (int mask = 0; mask < MAX_MASK; ++mask)
    {
        int d[DIGITS];
        for (int i = 0; i < DIGITS; ++i)
            d[i] = (mask >> (2 * (DIGITS - 1 - i))) & 3;

        int s[3];
        s[0] = d[0] + d[1] + d[2];
        s[1] = d[1] + d[2] + d[3];
        s[2] = d[2] + d[3] + d[4];
        if ((s[0] == 3 || s[0] == 6) &&
            (s[1] == 3 || s[1] == 6) &&
            (s[2] == 3 || s[2] == 6))
        {
            for (int i = 0; i < DIGITS; ++i)
                row_digits[M][i] = d[i];
            ++M;
        }
    }

    pairCount = M * M;
    dp     = (seq_t *)malloc(pairCount * sizeof(seq_t));
    newdp  = (seq_t *)malloc(pairCount * sizeof(seq_t));
    if (!dp || !newdp) exit(EXIT_FAILURE);

    /* 2) Count transitions and store flat array */
    size_t *cnt = (size_t *)calloc(pairCount, sizeof(size_t));
    if (!cnt) exit(EXIT_FAILURE);

    for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < M; ++j)
    {
        const int *ri = row_digits[i];
        const int *rj = row_digits[j];
        for (size_t k = 0; k < M; ++k)
        {
            const int *rk = row_digits[k];
            if (transition_ok(ri, rj, rk))
                ++cnt[i * M + j];
        }
    }

    trans_len  = cnt;   /* reuse memory for lengths */
    trans_start = (size_t *)malloc(pairCount * sizeof(size_t));
    if (!trans_start) exit(EXIT_FAILURE);

    /* prefix sums to get start offsets */
    trans_start[0] = 0;
    for (size_t p = 1; p < pairCount; ++p)
        trans_start[p] = trans_start[p - 1] + trans_len[p - 1];

    size_t total_trans = trans_start[pairCount - 1] + trans_len[pairCount - 1];
    trans_next = (int *)malloc(total_trans * sizeof(int));
    if (!trans_next) exit(EXIT_FAILURE);

    /* second pass: fill flat array */
    size_t *pos = (size_t *)malloc(pairCount * sizeof(size_t));
    if (!pos) exit(EXIT_FAILURE);
    for (size_t p = 0; p < pairCount; ++p) pos[p] = trans_start[p];

    for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < M; ++j)
    {
        const int *ri = row_digits[i];
        const int *rj = row_digits[j];
        size_t    base = i * M + j;
        for (size_t k = 0; k < M; ++k)
        {
            const int *rk = row_digits[k];
            if (transition_ok(ri, rj, rk))
                trans_next[pos[base]++] = (int)k;
        }
    }

    free(pos);

    /* 3) initialise DP tables: all 2‑row arrays are admissible */
    for (size_t idx = 0; idx < pairCount; ++idx)
        dp[idx] = (seq_t)1;

    /* 4) initialise memoisation cache */
    cache_len = 1;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = (seq_t)0;
    cached_n = 0;
}

/* -------------------------------------------------------------------- */
/* a(n) ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n ≥ 1 */

    if (!initialized) {
        init_once();
        initialized = true;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_len <= n)
    {
        size_t new_len = cache_len;
        while ((idx_t)new_len <= n) new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_len = new_len;
    }

    /* compute values up to n */
    for (idx_t step = cached_n + 1; step <= n; ++step)
    {
        /* zero newdp */
        for (size_t idx = 0; idx < pairCount; ++idx)
            newdp[idx] = (seq_t)0;

        /* transition */
        for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < M; ++j)
        {
            seq_t cur = dp[i * M + j];
            if (cur == (seq_t)0) continue;
            size_t base = i * M + j;
            size_t start = trans_start[base];
            size_t len   = trans_len[base];
            for (size_t t = 0; t < len; ++t)
            {
                int k = trans_next[start + t];
                newdp[j * M + k] += cur;
            }
        }

        /* swap dp and newdp */
        for (size_t idx = 0; idx < pairCount; ++idx)
            dp[idx] = newdp[idx];

        /* sum all dp entries to get a(step) */
        seq_t total = (seq_t)0;
        for (size_t idx = 0; idx < pairCount; ++idx)
            total += dp[idx];

        cache[step] = total;
    }

    cached_n = n;
    return cache[n];
}
