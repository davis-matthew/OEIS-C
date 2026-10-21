/*
 * A265755  a(n) = a(n-1) + a(n-2) if n is even and a(n) = a(n-3) + a(n-4) if
 *          n is odd, with a(0) = a(1) = a(2) = 0 and a(3) = 1.
 * https://oeis.org/A265755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) if n even; a(n) = a(n-3)+a(n-4) if n odd;
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 81, u64 n <= 155,
 *            u128 n <= 307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard library for memory allocation */
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *memo = NULL;      /* cache of computed values */
static size_t memo_len = 0;     /* number of computed entries */

/* Ensure that memo[n] exists.  Compute any missing entries lazily. */
static void
ensure_cache(idx_t n)
{
    /* If already computed, nothing to do. */
    if (n < memo_len)
        return;

    /* Allocate space for indices 0 .. n inclusive. */
    size_t new_len = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (!new_arr)
        abort();              /* allocation failed */

    memo = new_arr;

    /* Compute newly needed entries in increasing order. */
    for (size_t i = memo_len; i < new_len; ++i) {
        if (i <= 3) {                     /* base values */
            if (i == 0 || i == 1 || i == 2)
                memo[i] = 0;
            else                           /* i == 3 */
                memo[i] = 1;
        } else {
            if ((i & 1) == 0)             /* i even */
                memo[i] = memo[i-1] + memo[i-2];
            else                          /* i odd */
                memo[i] = memo[i-3] + memo[i-4];
        }
    }

    memo_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);   /* compute up to n if needed */
    return memo[n];
}
