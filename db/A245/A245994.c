/*
 * A245994  Number of length n+2 0..7 arrays with no pair in any consecutive
 *          three terms totalling exactly 7.
 * https://oeis.org/A245994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + a(n-2), with a(1) = 344, a(2) = 2120 (a(0) =
 *            56)
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t cache[10001];
static idx_t cache_n = 0;

/* Compute a(n) for 1 ≤ n. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the sequence, but return a(0) for safety */
        return (seq_t)56;

    /* initialise the cache on first use */
    if (cache_n == 0) {
        cache[0] = (seq_t)56;   /* we store a(0) for the recurrence */
        cache[1] = (seq_t)344;  /* a(1) */
        cache[2] = (seq_t)2120; /* a(2) */
        cache_n = 2;
    }

    /* extend the cache up to n if not already done */
    while (cache_n < n) {
        ++cache_n;
        cache[cache_n] = cache[cache_n - 1] * (seq_t)6 + cache[cache_n - 2];
    }

    return cache[n];
}
