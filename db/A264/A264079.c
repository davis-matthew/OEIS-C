/*
 * A264079  Expansion of b(2)*b(6)/(1 - 2*x + x^3 - x^4 - x^5 + 2*x^6), where
 *          b(k) = (1-x^k)/(1-x).
 * https://oeis.org/A264079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 69, u128 n <= 140,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (OEIS A264079):
     a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 21, a(4) = 41, a(5) = 79, a(6) = 150
     a(n) = 2*a(n-1) - a(n-3) + a(n-4) + a(n-5) - 2*a(n-6)   for n > 6
   The function memoizes computed values so that calls may be in any order.
*/

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *memo      = NULL;   /* array of computed terms */
static size_t memo_sz   = 0;      /* number of terms stored   */
static size_t memo_cap  = 0;      /* allocated capacity      */

/* Return the n-th term of the sequence (zero‑based indexing). */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;                 /* convert index for array use */
    size_t need = idx + 1;                  /* at least one slot for n          */
    if (need < 7) need = 7;                 /* ensure space for the base terms   */

    /* Ensure the array has enough capacity. */
    ensure_capacity(need, &memo_cap, (void **)&memo, sizeof(seq_t));

    /* Initialise the first seven terms only once. */
    if (memo_sz == 0) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)10;
        memo[3] = (seq_t)21;
        memo[4] = (seq_t)41;
        memo[5] = (seq_t)79;
        memo[6] = (seq_t)150;
        memo_sz = 7;
    }

    /* If the requested term is already known, return it. */
    if (idx < memo_sz) {
        return memo[idx];
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (size_t i = memo_sz; i <= idx; ++i) {
        memo[i] = 2 * memo[i-1]
                  - memo[i-3]
                  + memo[i-4]
                  + memo[i-5]
                  - 2 * memo[i-6];
    }

    memo_sz = idx + 1;          /* update the number of stored terms   */
    return memo[idx];
}
