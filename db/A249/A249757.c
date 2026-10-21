/*
 * A249757  Triangular array of coefficients of polynomials p(n,x) = (x +
 *          1)*p(n-1,x) + 2*n*x, p(0,x) = 1.
 * https://oeis.org/A249757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 158, u32 n <= 574, u64 n <= 2238,
 *            u128 n <= 5252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249757: flattened triangular array of coefficients of
   polynomials defined by
      p(0,x) = 1;
      p(n,x) = (x+1)*p(n-1,x) + 2*n*x .
   The term a(n) is the coefficient of x^k in row r of the triangle,
   where (r,k) is the unique pair with
      r*(r+1)/2 <= n < (r+1)*(r+2)/2   and   k = n - r*(r+1)/2.   */
#include <stdlib.h>
#include <stdint.h>

/* Cache for the flattened triangle */
static seq_t *cache      = NULL;   /* flattened values a(0),a(1),...    */
static size_t cache_cap  = 0;      /* allocated capacity                 */
static size_t cache_len  = 0;      /* number of values currently stored  */
static idx_t  rows_done  = 0;      /* last row index that has been
                                      fully generated (row 0 at start)   */

/* Ensure cache has at least `need` slots */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the sequence term a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache with the first row (row 0) if necessary */
    if (cache_len == 0) {
        ensure_cache_capacity(256);
        cache[0] = (seq_t)1;  /* c[0][0] = 1 */
        cache_len = 1;
        rows_done = 0;        /* last computed row is 0          */
    }

    /* Grow the triangle until we have the required index */
    while (cache_len <= n) {
        /* Next row to generate has index r = rows_done + 1          */
        idx_t r = rows_done + 1;                 /* current n for recurrence */
        idx_t prev_offset = rows_done * (rows_done + 1) / 2;  /* start of row r-1 */
        idx_t prev_len    = r;                                   /* length of row r-1 */

        /* Allocate space for the new entries */
        ensure_cache_capacity(cache_len + r + 1);

        /* k = 0, coefficient of x^0 */
        cache[cache_len++] = (seq_t)1;

        /* k = 1 .. r  (indexes of x^k in row r) */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t val = (seq_t)0;
            /* Add c[n-1][k] if it exists */
            if (k <= prev_len - 1) {
                val += cache[prev_offset + k];
            }
            /* Add c[n-1][k-1] (always exists) */
            val += cache[prev_offset + k - 1];

            /* Add the contribution from the +2*n*x term */
            if (k == 1) {
                val += (seq_t)(2 * r);
            }

            cache[cache_len++] = val;
        }

        /* Row r has now been fully generated */
        rows_done = r;
    }

    return cache[n];
}
