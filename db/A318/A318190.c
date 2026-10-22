/*
 * A318190  a(0) = 0, a(1) = 1; for n >= 1, a(2*n) = a(2*n-1) - 2*a(n),
 *          a(2*n+1) = 2*n - a(2*n).
 * https://oeis.org/A318190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 510, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0, a(1) = 1;
   a(2*n)   = a(2*n-1) - 2*a(n)
   a(2*n+1) = 2*n - a(2*n)
*/
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoization */
static seq_t *cache = NULL;    /* values */
static bool *used  = NULL;    /* whether value is computed */
static idx_t  cache_cap = 0;   /* current capacity */

/* Ensure cache can hold indices up to n */
static void ensure_cap(idx_t n) {
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    used  = (bool*)realloc(used,  new_cap * sizeof(bool));
    /* initialize new entries */
    for (idx_t i = cache_cap; i < new_cap; ++i) used[i] = false;
    cache_cap = new_cap;
}

/* Recursive computation with memoization */
static seq_t rec(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cap(n);
    if (used[n]) return cache[n];

    seq_t val;
    if ((n & 1) == 0) {           /* even: n = 2*k  */
        idx_t k = n >> 1;
        val = rec(n - 1) - 2 * rec(k);    /* a(2*k-1) - 2*a(k) */
    } else {                      /* odd:  n = 2*k+1 */
        idx_t k = (n - 1) >> 1;
        val = (seq_t)(2 * k) - rec(n - 1); /* 2*k - a(2*k) */
    }

    cache[n] = val;
    used[n]  = true;
    return val;
}

/* Public entry point */
seq_t a(idx_t n) {
    return rec(n);
}
