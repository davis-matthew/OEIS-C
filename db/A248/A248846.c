/*
 * A248846  Triangle read by rows: T(m,n) = number of ways of distributing n
 *          distinguishable balls into m distinguishable bins of size 4 where
 *          empty bins are permitted (m >= 1, 1 <= n <= 4m).
 * https://oeis.org/A248846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(m,k) where * 1 ≤ n, let m = smallest integer with
 *            ∑_{i=1}^m 4i ≥ n, * k = n − ∑_{i=1}^{m−1} 4i. * T(m,k) counts
 *            assignments of k labeled balls into m labeled bins of * capacity
 *            4 (empty bins allowed). * Recurrence: T(m,0)=1 and for k>0 *
 *            T(m,k) = Σ_{t=0..min(4,k)} binom(k,t)·T(m−1,k−t)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic storage of rows of the triangle */
static seq_t **rows      = NULL;   /* rows[m] -> array of T(m,0..4m) */
static size_t rows_len   = 0;      /* number of rows allocated (including m=0) */
static size_t rows_cap   = 0;      /* capacity of rows array */

static idx_t *cum        = NULL;   /* cumulative sums cum[m] = ∑_{i=1}^m 4i, cum[0]=0 */
static size_t cum_len    = 0;      /* number of cum entries allocated (>= rows_len) */
static size_t cum_cap    = 0;      /* capacity of cum array */

/* Small binomial helper for 0≤t≤4 */
static inline seq_t binom_small(seq_t k, unsigned t)
{
    seq_t res = 1;
    for (unsigned i = 1; i <= t; ++i) {
        res = res * (k - i + 1) / i;
    }
    return res;
}

/* Ensure capacity of rows array */
static void ensure_rows_cap(size_t needed)
{
    if (needed <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(*tmp));
    if (!tmp) abort();
    rows = tmp;
    rows_cap = new_cap;
}

/* Ensure capacity of cum array */
static void ensure_cum_cap(size_t needed)
{
    if (needed <= cum_cap) return;
    size_t new_cap = cum_cap ? cum_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(*tmp));
    if (!tmp) abort();
    cum = tmp;
    cum_cap = new_cap;
}

/* Initialise storage with dummy row 0 (not used for a(n)) */
static void init_storage(void)
{
    if (rows_len) return;
    ensure_rows_cap(1);
    ensure_cum_cap(1);
    rows[0] = (seq_t *)malloc(sizeof(seq_t));
    if (!rows[0]) abort();
    rows[0][0] = 1;          /* T(0,0)=1 */
    rows_len = 1;
    cum[0]    = 0;
    cum_len   = 1;
}

/* Extend rows until cum[m] ≥ target_n */
static void extend_rows_until(idx_t target_n)
{
    init_storage();
    while (cum_len - 1 < target_n) {
        size_t m = cum_len;                /* next row number (m≥1) */
        size_t cur_max = 4 * m;            /* k ranges 0..4m */
        size_t prev_max = 4 * (m - 1);

        seq_t *row = (seq_t *)malloc((cur_max + 1) * sizeof(seq_t));
        if (!row) abort();
        row[0] = 1;                        /* T(m,0)=1 */

        for (size_t k = 1; k <= cur_max; ++k) {
            unsigned t_low  = (k > prev_max) ? (unsigned)(k - prev_max) : 0;
            unsigned t_high = (k < 4) ? (unsigned)k : 4;
            seq_t sum = 0;
            for (unsigned t = t_low; t <= t_high; ++t) {
                seq_t comb = binom_small((seq_t)k, t);
                seq_t prod = comb * rows[m - 1][k - t];
                sum += prod;
            }
            row[k] = sum;
        }

        ensure_rows_cap(m + 1);
        rows[m] = row;
        rows_len = m + 1;

        if (cum_len <= m) {
            ensure_cum_cap(m + 1);
            cum[m] = cum[m - 1] + cur_max;
            cum_len = m + 1;
        } else {
            cum[m] = cum[m - 1] + cur_max;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below offset */
    extend_rows_until(n);           /* ensure we have rows covering n */

    /* Find row m such that cum[m-1] < n ≤ cum[m] */
    idx_t lo = 1, hi = cum_len - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;
    idx_t k = n - cum[m - 1];

    return rows[m][(size_t)k];
}
