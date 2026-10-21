/*
 * A263113  Number of (n+2)X(1+2) 0..1 arrays with each row and column not
 *          divisible by 7, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 binary matrices whose rows * and
 *            columns (read as binary integers from top/left) are not *
 *            divisible by 7. This implementation uses a DP over the *
 *            column‑residue triplet modulo 7
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Pre‑computed transition tables.  343 = 7³ possible column residues. */
static int  trans[343][6];
static bool all_nonzero[343];
static bool tables_initialized = false;

/* All admissible 3‑bit row patterns (000 and 111 are forbidden). */
static const int row_bits[6][3] = {
    {0,0,1},   /* 001 */
    {0,1,0},   /* 010 */
    {0,1,1},   /* 011 */
    {1,0,0},   /* 100 */
    {1,0,1},   /* 101 */
    {1,1,0}    /* 110 */
};

/* Build the transition table once. */
static void init_tables(void)
{
    if (tables_initialized)
        return;
    tables_initialized = true;

    for (int a = 0; a < 7; a++)
        for (int b = 0; b < 7; b++)
            for (int c = 0; c < 7; c++) {
                int s = a*49 + b*7 + c;
                all_nonzero[s] = (a != 0 && b != 0 && c != 0);
                for (int r = 0; r < 6; r++) {
                    int na = (a*2 + row_bits[r][0]) % 7;
                    int nb = (b*2 + row_bits[r][1]) % 7;
                    int nc = (c*2 + row_bits[r][2]) % 7;
                    trans[s][r] = na*49 + nb*7 + nc;
                }
            }
}

/* DP state for the current number of processed rows.  */
static seq_t dp[343]      = {0};
static seq_t newdp[343]   = {0};
static idx_t current_rows = 0;   /* rows already processed */
static idx_t max_cached   = 0;   /* largest n for which a(n) is cached */

static seq_t *cache   = NULL;   /* cache[n] holds a(n) (1‑based) */
static size_t cache_len = 0;     /* allocated cache length */

/* Grow the cache so that it can hold at least indices 0..n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len)
        return;
    size_t new_len = ((size_t)n + 1) * 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_len = new_len;
}

/* ------------------------------------------------------------ */
/*  seq_t a(idx_t n) implementation                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)  /* outside of defined domain */
        return 0;

    init_tables();

    /* Initialise DP and cache only once */
    static bool initialized = false;
    if (!initialized) {
        memset(dp, 0, sizeof(dp));
        dp[0] = 1;          /* start with all column residues = 0 */
        current_rows = 0;
        ensure_cache(20);   /* initial cache capacity */
        initialized = true;
    }

    /* We need (n+2) rows to compute a(n). */
    idx_t target_rows = n + 2;
    while (current_rows < target_rows) {
        /* Transition: add one more row */
        memset(newdp, 0, sizeof(newdp));

        for (int s = 0; s < 343; s++) {
            seq_t val = dp[s];
            if (val == 0)
                continue;
            for (int r = 0; r < 6; r++) {
                int ns = trans[s][r];
                newdp[ns] = newdp[ns] + val;
            }
        }

        memcpy(dp, newdp, sizeof(dp));
        current_rows++;

        /* Record value for this n once at least 3 rows have been built
           (because n = current_rows - 2 >= 1). */
        if (current_rows >= 3) {
            idx_t this_n = current_rows - 2;
            seq_t sum = 0;
            for (int s = 0; s < 343; s++)
                if (all_nonzero[s])
                    sum = sum + dp[s];
            ensure_cache(this_n);
            cache[this_n] = sum;
            if (this_n > max_cached)
                max_cached = this_n;
        }
    }

    return cache[n];
}
