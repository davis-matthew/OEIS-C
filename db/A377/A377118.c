/*
 * A377118  a(n) = coefficient of 2^(1/3) in the expansion of (2^(1/3) +
 *          2^(2/3))^n.
 * https://oeis.org/A377118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 6*a(n-3), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental memoization cache.
   It works for any integer type seq_t that supports the ordinary
   arithmetic operators (+, -, *, /, % and integral shifts) and
   any idx_t type that is a C unsigned integer (typically uint64_t). */

#include <stdlib.h>   /* malloc, realloc, NULL */

static seq_t *a_cache = NULL;   /* cache for computed values   */
static idx_t a_cap   = 0;       /* current allocated capacity  */
static idx_t a_used  = 0;       /* number of initialized slots */

/* Ensure the cache has space for indices up to n (inclusive). */
static void a_ensure(idx_t n)
{
    if (n + 1 <= a_cap) return;

    idx_t new_cap = a_cap ? a_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;

    /* realloc works fine for NULL pointer, returns a pointer to the
       new area or NULL on failure. The cast is required for C++ but
       has no effect in C. */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the trivial base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    a_ensure(n);

    /* If the cache has not yet been initialised, seed the first
       three values.  This is done only once. */
    if (a_used == 0) {
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)2;
        a_used    = 3;
    }

    /* Compute any values that are still missing up to n. */
    if (n >= a_used) {
        for (idx_t i = a_used; i <= n; ++i) {
            /* a(i) = 6*a(i-2) + 6*a(i-3) for i >= 3 */
            a_cache[i] = (seq_t)6 * a_cache[i - 2] + (seq_t)6 * a_cache[i - 3];
        }
        a_used = n + 1;
    }

    return a_cache[n];
}
