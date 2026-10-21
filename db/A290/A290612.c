/*
 * A290612  Number of maximal independent vertex sets (and minimal vertex
 *          covers) in the n-wheel graph.
 * https://oeis.org/A290612
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) Initial values: a(4)=4, a(5)=3,
 *            a(6)=6, a(7)=6
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_CACHE_N 10000

static seq_t cache[MAX_CACHE_N + 1];
static idx_t cache_max = 3;

seq_t a(idx_t n) {
    if (n < 4)
        return 0;

    /* Build the cache up to MAX_CACHE_N once */
    if (cache_max < MAX_CACHE_N) {
        for (idx_t i = cache_max + 1; i <= MAX_CACHE_N; ++i) {
            seq_t val;
            if (i == 4)
                val = (seq_t)4;
            else if (i == 5)
                val = (seq_t)3;
            else if (i == 6)
                val = (seq_t)6;
            else if (i == 7)
                val = (seq_t)6;
            else
                val = cache[i - 1] + cache[i - 2] - cache[i - 4];
            cache[i] = val;
        }
        cache_max = MAX_CACHE_N;
    }

    if (n <= MAX_CACHE_N) {
        return cache[n];
    }

    /* For n larger than MAX_CACHE_N, compute on the fly using a sliding window
       that stores only the last four values needed by the recurrence.      */
    seq_t win[4];
    win[0] = cache[MAX_CACHE_N];     /* a[MAX_CACHE_N]   */
    win[1] = cache[MAX_CACHE_N - 1]; /* a[MAX_CACHE_N-1] */
    win[2] = cache[MAX_CACHE_N - 2]; /* a[MAX_CACHE_N-2] */
    win[3] = cache[MAX_CACHE_N - 3]; /* a[MAX_CACHE_N-3] */

    for (idx_t i = MAX_CACHE_N + 1; i <= n; ++i) {
        seq_t val = win[0] + win[1] - win[3];
        win[3] = win[2];
        win[2] = win[1];
        win[1] = win[0];
        win[0] = val;
    }
    return win[0];
}
