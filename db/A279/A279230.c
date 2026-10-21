/*
 * A279230  Expansion of 1/((1-x)^2*(1-2*x+2*x^2)).
 * https://oeis.org/A279230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + n + 1, a(0)=1, a(1)=4, a(2)=9,
 *            a(3)=14
 * Range:     i8 n <= 10, i16 n <= 26, i32 n <= 58, i64 n <= 122,
 *            i128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;               /* cached values             */
    static idx_t  memo_len = 0;               /* number of values stored   */
    static size_t memo_cap = 0;               /* current capacity (in elems)*/

    /* Initialise the first four values on first call. */
    if (memo_len == 0) {
        memo_cap = 4;
        memo = (seq_t*)malloc(memo_cap * sizeof(seq_t));
        if (!memo) abort();

        memo[0] = 1;   /* a(0) */
        memo[1] = 4;   /* a(1) */
        memo[2] = 9;   /* a(2) */
        memo[3] = 14;  /* a(3) */
        memo_len = 4;
    }

    /* If the requested n is already cached, return it immediately. */
    if (n < memo_len)
        return memo[n];

    /* Grow the cache if necessary.  Double the capacity each time. */
    if ((size_t)(n + 1) > memo_cap) {
        size_t new_cap = memo_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute values from memo_len up to n using the recurrence. */
    for (idx_t i = memo_len; i <= n; ++i) {
        seq_t val = 2 * memo[i - 1] - 2 * memo[i - 2] + (seq_t)i + 1;
        memo[i] = val;
    }

    memo_len = n + 1;
    return memo[n];
}
