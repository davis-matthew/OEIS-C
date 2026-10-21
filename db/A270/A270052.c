/*
 * A270052  Number of nX2 0..4 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling four exactly once.
 * https://oeis.org/A270052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=240, a(3)=5400; for n>3: a(n)=26*a(n-1)-169*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Memoization table for the sequence */
static seq_t *memo = NULL;
static size_t memo_cap = 0;
static idx_t memo_len = 0;

/* Ensure the cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;
    if (n <= memo_len)
        return;

    /* Grow the storage to accommodate n entries. */
    size_t needed = (size_t)n;
    size_t new_cap = memo_cap ? memo_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;   /* double each time */

    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */

    memo = tmp;
    memo_cap = new_cap;

    /* Fill in the missing values. */
    for (idx_t i = memo_len; i < n; ++i) {
        if (i == 0)
            memo[i] = (seq_t)5;
        else if (i == 1)
            memo[i] = (seq_t)240;
        else if (i == 2)
            memo[i] = (seq_t)5400;
        else
            memo[i] = 26 * memo[i - 1] - (seq_t)169 * memo[i - 2];
    }

    memo_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset < 1 */

    ensure_cache(n);
    return memo[n - 1];
}
