/*
 * A396887  a(n) = (n-3)*a(n-1) + (n-1)*a(n-3); a(0) = 0, a(1) = 0, a(2) = 1.
 * https://oeis.org/A396887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (n-3)*a(n-1) + (n-1)*a(n-3) for n≥3, with
   a(0)=0, a(1)=0, a(2)=1. This function memoizes results to allow
   arbitrary order of calls. */
#include <stdlib.h>

static seq_t *memo = NULL;          /* cache of computed terms */
static idx_t memo_size = 0;         /* current allocated size (terms 0..memo_size-1) */

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n < 3) {
        return (n == 2) ? (seq_t)1 : (seq_t)0;
    }

    /* grow the cache if necessary */
    if (memo_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed – terminate */
        memo = tmp;

        /* compute missing terms */
        for (idx_t i = memo_size; i <= n; ++i) {
            if (i < 3) { /* base values */
                memo[i] = (i == 2) ? (seq_t)1 : (seq_t)0;
            } else {
                seq_t term = (seq_t)(i - 3) * memo[i - 1]
                           + (seq_t)(i - 1) * memo[i - 3];
                memo[i] = term;
            }
        }
        memo_size = new_size;
    }

    return memo[n];
}
