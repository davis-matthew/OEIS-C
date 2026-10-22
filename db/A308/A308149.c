/*
 * A308149  Positive integers with Collatz trajectories that do not include
 *          the number 5.
 * https://oeis.org/A308149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308149: positive integers whose Collatz trajectories never
   contain the value 5.  Computed by brute‑force search of the Collatz
   trajectory for each candidate integer. */
#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the Collatz trajectory of x hits 5 before reaching 1. */
static bool contains5(seq_t x)
{
    while (x != (seq_t)1) {
        if (x == (seq_t)5)
            return true;
        if ((x & 1) == 0)
            x = x >> 1;          /* shift right by 1 bit */
        else
            x = x * 3 + 1;       /* 3x + 1 */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of the first `found` terms. */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t found     = 0;
    static seq_t cur        = (seq_t)1;   /* next candidate to test */

    /* The sequence is defined for n >= 1; return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    if (found >= n)
        return cache[(size_t)(n - 1)];

    while (found < n) {
        if (!contains5(cur)) {
            /* Expand cache if necessary. */
            if (found >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                if (new_cap < found + 1)
                    new_cap = found + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[found] = cur;
            ++found;
        }
        cur = cur + (seq_t)1;   /* move to next integer */
    }

    return cache[(size_t)(n - 1)];
}
