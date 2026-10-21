/*
 * A279673  The maximum number of coins that can be processed in n weighings
 *          where all coins are real except for one LHR-coin starting in the
 *          light state.
 * https://oeis.org/A279673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3); a(0) = 1, a(1) = 3, a(2) =
 *            9
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* array of computed values   */
static size_t  a_cache_cap = 0;    /* allocated capacity         */
static idx_t   a_cache_len = 0;    /* number of values actually filled */

/* Ensure the array has room for at least indices 0..n */
static void a_ensure(idx_t n)
{
    if ((size_t)n + 1 <= a_cache_cap) return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 16) new_cap = 16;          /* small initial growth */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);           /* memory allocation failed */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Compute all values up to and including n, storing them in a_cache */
static void a_compute_upto(idx_t n)
{
    a_ensure(n);

    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = 1;
        } else if (i == 1) {
            a_cache[1] = 3;
        } else if (i == 2) {
            a_cache[2] = 9;
        } else {
            a_cache[i] = (seq_t)2 * a_cache[i-1] - a_cache[i-2] + (seq_t)4 * a_cache[i-3];
        }
    }
    a_cache_len = n + 1;
}

seq_t a(idx_t n)
{
    a_compute_upto(n);
    return a_cache[n];
}
