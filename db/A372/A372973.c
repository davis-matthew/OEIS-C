/*
 * A372973  Triangle read by rows: the exponential almost-Riordan array (
 *          1/(1-x) | 1/(1-x), log(1/(1-x)) ).
 * https://oeis.org/A372973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2; T(r,0) = r!; for c>0, T(r,c) = unsigned Stirling
 *            number of the first kind s(r,c)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *fact = NULL;           /* factorials, fact[i] = i!      */
static idx_t  fact_size = 0;         /* current factorial array size  */

static seq_t *stirl_prev = NULL;     /* previous row of Stirling numbers */
static seq_t *stirl_curr = NULL;     /* current row of Stirling numbers  */
static size_t stirl_max = 0;         /* allocated width of stirling arrays */

static seq_t *cache = NULL;          /* flattened triangle values      */
static idx_t  cache_size = 0;        /* number of elements stored in cache */

static idx_t  max_row_computed = 0;  /* last row fully computed (>=0) */

/* Ensure factorial array, stirling arrays and cache grow enough
   to hold values up to the given row (<= target_row).  Compute
   all intermediate rows and store their triangle values in 'cache'. */
static void ensure_rows_upto_row(idx_t target_row)
{
    /* Initial allocation */
    if (fact_size == 0) {
        fact = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!fact) abort();
        fact[0] = (seq_t)1;
        fact_size = 1;

        stirl_prev = (seq_t *)malloc(1 * sizeof(seq_t));
        stirl_curr = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!stirl_prev || !stirl_curr) abort();
        stirl_prev[0] = (seq_t)1;      /* s(0,0)=1 */
        stirl_max = 1;

        cache = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_size = 1;
        max_row_computed = 0;
    }

    if (target_row <= max_row_computed) return;

    /* Grow factorial array */
    if (fact_size < (size_t)target_row + 1) {
        size_t new_fact_size = (size_t)target_row + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, new_fact_size * sizeof(seq_t));
        if (!new_fact) abort();
        for (size_t i = fact_size; i < new_fact_size; ++i) {
            new_fact[i] = new_fact[i - 1] * (seq_t)(i);
        }
        fact = new_fact;
        fact_size = new_fact_size;
    }

    /* Grow stirling arrays to hold rows up to target_row */
    if (stirl_max < (size_t)target_row + 1) {
        size_t new_max = (size_t)target_row + 1;
        seq_t *new_prev = (seq_t *)realloc(stirl_prev, new_max * sizeof(seq_t));
        seq_t *new_curr = (seq_t *)realloc(stirl_curr, new_max * sizeof(seq_t));
        if (!new_prev || !new_curr) abort();
        stirl_prev = new_prev;
        stirl_curr = new_curr;
        stirl_max = new_max;
    }

    /* Grow cache to hold all entries up to the target row */
    size_t needed_cache = ((size_t)target_row + 1) * ((size_t)target_row + 2) / 2;
    if (cache_size < needed_cache) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed_cache * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_size = needed_cache;
    }

    /* Compute rows one by one from current+1 up to target_row */
    for (idx_t row = max_row_computed + 1; row <= target_row; ++row) {
        /* Stirling numbers for this row */
        stirl_curr[0] = (seq_t)0;            /* unused, s(row,0)=0 for row>0 */
        for (size_t k = 1; k <= (size_t)row; ++k) {
            seq_t left = stirl_prev[k - 1];
            seq_t right = (k <= (size_t)row - 1) ? stirl_prev[k] : (seq_t)0;
            seq_t mult = right * (seq_t)(row - 1);
            stirl_curr[k] = left + mult;
        }

        /* Store triangle values to cache */
        idx_t base = row * (row + 1) / 2;     /* first index of this row */
        cache[base] = fact[row];             /* T(row,0) = row!      */
        for (size_t k = 1; k <= (size_t)row; ++k) {
            cache[base + k] = stirl_curr[k]; /* T(row,k) = s(row,k)   */
        }

        /* Prepare for next iteration: swap rows */
        seq_t *tmp = stirl_prev;
        stirl_prev = stirl_curr;
        stirl_curr = tmp;
    }

    max_row_computed = target_row;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t col = n - r * (r + 1) / 2;   /* column index within the row */

    ensure_rows_upto_row(r);

    idx_t base = r * (r + 1) / 2;
    return cache[base + col];
}
