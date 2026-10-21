/*
 * A252284  Exponential generating function exp(-x-x^2-x^3/3).
 * https://oeis.org/A252284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = -a(n+2) - 2*(n+2)*a(n+1) - (n+2)*(n+1)*a(n)
   Initial values: a(0)=1, a(1)=-1, a(2)=-1, a(3)=3
   This implements the OEIS sequence A252284. */

#include <stddef.h>             /* for size_t */
#include "../utils/ensure_capacity.h"

static seq_t *memo = NULL;
static size_t memo_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure space for at least indices 0..3 (recurrence needs them) */
    size_t needed = (size_t)n + 1;
    if (needed < 4) needed = 4;
    ensure_capacity(needed, &memo_cap, (void **)&memo, sizeof(seq_t));

    /* Set the fixed initial values (overwriting if already present is safe) */
    memo[0] = (seq_t)1;
    memo[1] = (seq_t)-1;
    memo[2] = (seq_t)-1;
    memo[3] = (seq_t)3;

    /* Compute next terms if needed */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = -memo[i - 1];
        seq_t mult1 = (seq_t)(i - 1);
        val -= (seq_t)2 * mult1 * memo[i - 2];
        seq_t mult2 = mult1 * (seq_t)(i - 2); /* (i-1)*(i-2) */
        val -= mult2 * memo[i - 3];
        memo[i] = val;
    }
    return memo[n];
}
