/*
 * A373933  Number of compositions of 7*n-1 into parts 6 and 7.
 * https://oeis.org/A373933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of 7*n-1 into parts 6 and 7.
 *            Equivalent to f(0)=1 and for m≥1: f(m)=f(m-6)+f(m-7) (treat
 *            missing terms as 0). Then a(n)=f(7*n-1). This uses only
 *            additions on unsigned types, so it works for all provided seq_t
 *            definitions
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *f_cache = NULL;   /* array for f(m) */
static size_t  f_cap   = 0;     /* current capacity of f_cache (in seq_t) */
static idx_t   f_max_m = 0;     /* highest m computed so far */

/* Ensure f_cache has capacity for indices up to target_m (inclusive). */
static void ensure_f_capacity(idx_t target_m)
{
    size_t needed = (size_t)(target_m + 1);
    if (!f_cache) {
        /* First allocation: start with capacity 1 and grow as needed. */
        f_cap = 1;
        while (f_cap < needed)
            f_cap <<= 1;
        f_cache = (seq_t *)malloc(f_cap * sizeof(seq_t));
        if (!f_cache) abort();          /* fatal error on allocation failure */
        f_cache[0] = (seq_t)1;          /* f(0)=1 */
        f_max_m = 0;
    } else if (needed > f_cap) {
        size_t new_cap = f_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        f_cache = new_arr;
        f_cap   = new_cap;
    }
}

/* Compute f(m) up to target_m using the recurrence f(m)=f(m-6)+f(m-7). */
static void compute_f_to(idx_t target_m)
{
    ensure_f_capacity(target_m);
    for (idx_t m = f_max_m + 1; m <= target_m; ++m) {
        seq_t val = 0;
        if (m >= 6)
            val += f_cache[m - 6];
        if (m >= 7)
            val += f_cache[m - 7];
        f_cache[m] = val;
    }
    f_max_m = target_m;
}

/* Public sequence function required by the framework. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    idx_t target_m = 7 * n - 1;          /* N = 7*n-1 */
    compute_f_to(target_m);
    return f_cache[target_m];
}
