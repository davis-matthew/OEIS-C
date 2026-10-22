/*
 * A336589  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(x) * BesselI(0,2*sqrt(1 -
 *          exp(x))).
 * https://oeis.org/A336589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A336589
   Formula: a(n) = n! * Sum_{k=0..n} (-1)^k * Stirling2(n+1,k+1) / k!  */

/*  The implementation keeps factorials and Stirling‑2 rows in
    static caching arrays so that repeated calls return in O(1)
    after initial O(n) build.  The critical change from the buggy
    version is that each term is computed as
        (n! / k!) * S(n+1,k+1)
    with the division performed before the multiplication to
    avoid 64‑bit overflow on intermediate results.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- generic helpers ---------- */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static void grow_seq_array(seq_t **arr, size_t *cap, size_t new_cap) {
    seq_t *tmp = alloc_seq_array(new_cap);
    if (*arr != NULL) {
        size_t len = (*cap < new_cap) ? *cap : new_cap;
        for (size_t i = 0; i < len; ++i)
            tmp[i] = (*arr)[i];
        free_seq_array(*arr);
    }
    *arr = tmp;
    *cap = new_cap;
}

/* ---------- static cache state ---------- */
static seq_t       *fact         = NULL;    /* factorials: fact[i] = i!      */
static size_t        fact_len     = 0;
static size_t        fact_cap     = 0;

static idx_t         cur_m        = 0;      /* current Stirling row index   */
static seq_t        *row          = NULL;    /* S(m,j) for current m         */
static seq_t        *next_row     = NULL;
static size_t        row_cap      = 0;
static size_t        next_cap     = 0;

static seq_t       *cache        = NULL;    /* memoised a(n) values        */
static size_t        cache_cap    = 0;
static size_t        cache_len    = 0;

/* ---------- auxiliary routine ---------- */
static void ensure_cache(idx_t n)
{
    /* ----- factorials up to n ----- */
    if (fact == NULL) {
        fact     = alloc_seq_array(1);
        fact[0]  = 1;
        fact_len = 1;
        fact_cap = 1;
    }
    while (fact_len <= (size_t)n) {
        if (fact_cap < fact_len + 1)
            grow_seq_array(&fact, &fact_cap, fact_len + 1);
        fact[fact_len] = fact[fact_len - 1] * (seq_t)fact_len;
        ++fact_len;
    }

    /* ----- Stirling‑2 rows up to m = n+1 ----- */
    if (row == NULL) {
        row      = alloc_seq_array(1);
        row[0]   = 1;               /* S(0,0) = 1 */
        row_cap  = 1;
        cur_m    = 0;
        next_row = alloc_seq_array(1);
        next_cap = 1;
    }
    idx_t target_m = n + 1;          /* need row for S(n+1,*) */
    while (cur_m < target_m) {
        idx_t old_m = cur_m;
        ++cur_m;                    /* move to next row, m = cur_m */

        idx_t new_size = cur_m + 1;  /* indices 0 … cur_m */
        if (row_cap < new_size) {
            grow_seq_array(&row,      &row_cap,  new_size);
            grow_seq_array(&next_row, &next_cap, new_size);
        }

        next_row[0] = 0;                 /* S(m,0) = 0 for m>0 */
        for (idx_t j = 1; j <= cur_m; ++j) {
            seq_t a_val = (j <= old_m) ? row[j] : 0;
            seq_t b_val = (j - 1 <= old_m) ? row[j - 1] : 0;
            next_row[j] = (seq_t)j * a_val + b_val;
        }

        for (idx_t j = 0; j <= cur_m; ++j)
            row[j] = next_row[j];
    }

    /* ----- cache growth ----- */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* ----- compute missing a(m) values ----- */
    for (idx_t mreq = cache_len; mreq <= n; ++mreq) {
        seq_t sum = 0;
        seq_t fact_n = fact[mreq];         /* n! */
        for (idx_t j = 1; j <= mreq + 1; ++j) {
            /* compute n! / (j-1)! first, then multiply by S(n+1,j) */
            seq_t fact_div = fact_n / fact[j - 1];
            seq_t term     = fact_div * row[j];
            if ((j - 1) & 1)
                sum -= term;
            else
                sum += term;
        }
        cache[mreq] = sum;
    }
    cache_len = n + 1;
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
