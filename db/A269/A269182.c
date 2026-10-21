/*
 * A269182  Number of nX4 0..3 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling three exactly once.
 * https://oeis.org/A269182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46*a(n-1) -607*a(n-2) +1264*a(n-3) +11029*a(n-4)
 *            -27640*a(n-5) -86627*a(n-6) +153412*a(n-7) +308464*a(n-8)
 *            -329306*a(n-9) -438615*a(n-10) +260680*a(n-11) +134834*a(n-12)
 *            -65824*a(n-13) -9889*a(n-14) +5080*a(n-15) -400*a(n-16) for n>17
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n <= 17 the values are computed by a transfer‑matrix search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Function prototype supplied by the build */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Helper tables for the transfer matrix                                     */
static bool tables_initialized = false;
static uint8_t inhor[256];                  /* horizontal sum==3 in a row */
static uint8_t between[256][256];           /* vertical+diagonal sum==3 */

/* --------------------------------------------------------------------- */
/* Initialize adjacency tables (runs once)                                 */
static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = true;

    for (int cfg = 0; cfg < 256; ++cfg) {
        int cell[4];
        int tmp = cfg;
        for (int j = 0; j < 4; ++j) {
            cell[j] = tmp & 3;
            tmp >>= 2;
        }
        /* horizontal within row */
        uint8_t h = 0;
        for (int j = 0; j < 3; ++j)
            if (cell[j] + cell[j + 1] == 3) ++h;
        inhor[cfg] = h;

        /* between two rows */
        for (int cfg2 = 0; cfg2 < 256; ++cfg2) {
            int cell2[4];
            int tmp2 = cfg2;
            for (int j = 0; j < 4; ++j) {
                cell2[j] = tmp2 & 3;
                tmp2 >>= 2;
            }
            uint8_t d = 0;
            for (int j = 0; j < 4; ++j) {
                if (cell[j] + cell2[j] == 3) ++d;          /* vertical */
                if (j > 0 && cell[j] + cell2[j - 1] == 3) ++d; /* diagonal left */
                if (j < 3 && cell[j] + cell2[j + 1] == 3) ++d; /* diagonal right */
            }
            between[cfg][cfg2] = d;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Cache of computed a(n) values (lazily expanded)                        */
static seq_t *cache = NULL;   /* cache[0] unused, cache[1] is a(1), etc. */
static idx_t cache_size = 0;  /* highest n for which cache[n] is valid */

/* DP state for rows 1..17                                 */
static seq_t dp[2][256][2];   /* dp[cur][cfg][cnt] */
static int dp_cur_idx = 0;    /* index of current dp in dp[2] */
static int dp_next_idx = 1;   /* index of next dp in dp[2] */

/* --------------------------------------------------------------------- */
/* Recurrence coefficients (index 1..16)                                 */
static const long long coeff[17] = {
    0,           /* unused index 0 */
    46LL,
   -607LL,
   1264LL,
   11029LL,
  -27640LL,
  -86627LL,
  153412LL,
  308464LL,
 -329306LL,
 -438615LL,
  260680LL,
  134834LL,
  -65824LL,
  -9889LL,
   5080LL,
  -400LL
};

/* --------------------------------------------------------------------- */
/* Compute cache up to n (inclusive)                                       */
static void init_cache(idx_t nmax)
{
    if (cache_size >= nmax) return;

    /* Extend cache array */
    seq_t *tmp = (seq_t *)realloc(cache, (nmax + 1) * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    cache = tmp;
    /* ensure new entries are zero-initialized */
    for (idx_t i = cache_size + 1; i <= nmax; ++i) cache[i] = (seq_t)0;

    init_tables();

    /* ------------------------------------------------------------------ */
    /* Compute values up to min(nmax,17) by transfer matrix               */
    idx_t first = cache_size + 1;
    idx_t last_dp = (nmax < 17) ? nmax : 17;

    for (idx_t row = first; row <= last_dp; ++row) {
        if (row == 1) {
            /* initialise dp for first row */
            for (int cfg = 0; cfg < 256; ++cfg) {
                dp[dp_cur_idx][cfg][0] = (seq_t)0;
                dp[dp_cur_idx][cfg][1] = (seq_t)0;
            }
            for (int cfg = 0; cfg < 256; ++cfg) {
                uint8_t h = inhor[cfg];
                if (h == 0) dp[dp_cur_idx][cfg][0] += (seq_t)1;
                else if (h == 1) dp[dp_cur_idx][cfg][1] += (seq_t)1;
            }
            /* sum of ones gives a(1) */
            seq_t sum = (seq_t)0;
            for (int cfg = 0; cfg < 256; ++cfg) sum += dp[dp_cur_idx][cfg][1];
            cache[1] = sum;
        } else {
            /* zero next dp table */
            for (int cfg = 0; cfg < 256; ++cfg) {
                dp[dp_next_idx][cfg][0] = (seq_t)0;
                dp[dp_next_idx][cfg][1] = (seq_t)0;
            }
            /* transition from dp_cur to dp_next */
            for (int prev = 0; prev < 256; ++prev) {
                seq_t val0 = dp[dp_cur_idx][prev][0];
                if (val0 != (seq_t)0) {
                    for (int cur_cfg = 0; cur_cfg < 256; ++cur_cfg) {
                        uint8_t add = inhor[cur_cfg] + between[prev][cur_cfg];
                        if (add == 0) dp[dp_next_idx][cur_cfg][0] += val0;
                        else if (add == 1) dp[dp_next_idx][cur_cfg][1] += val0;
                    }
                }
                seq_t val1 = dp[dp_cur_idx][prev][1];
                if (val1 != (seq_t)0) {
                    for (int cur_cfg = 0; cur_cfg < 256; ++cur_cfg) {
                        uint8_t add = inhor[cur_cfg] + between[prev][cur_cfg];
                        if (add == 0) dp[dp_next_idx][cur_cfg][1] += val1;
                    }
                }
            }
            /* sum of dp_next ones gives a(row) */
            seq_t sum = (seq_t)0;
            for (int cfg = 0; cfg < 256; ++cfg) sum += dp[dp_next_idx][cfg][1];
            cache[row] = sum;

            /* swap dp arrays for next iteration */
            int tmp_idx = dp_cur_idx;
            dp_cur_idx = dp_next_idx;
            dp_next_idx = tmp_idx;
        }
        cache_size = row;
    }

    /* ------------------------------------------------------------------ */
    /* Compute remaining terms using the linear recurrence                */
    for (idx_t row = last_dp + 1; row <= nmax; ++row) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int k = 1; k <= 16; ++k) {
            seq_t term = cache[row - k];
            long long c = coeff[k];
            if (c > 0) pos += term * (seq_t)c;
            else       neg += term * (seq_t)(-c);
        }
        cache[row] = pos - neg;
        cache_size = row;
    }
}

/* --------------------------------------------------------------------- */
/* Public function                                                          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_cache(n);
    return cache[n];
}
