/*
 * A251148  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5), initial terms:
 *            a(1)=853, a(2)=1075, a(3)=1413, a(4)=1987, a(5)=2925
 * Range:     u8 none, u16 n <= 11, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cached = 0;

/* compute the n-th term of A251148 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset 1; behaviour for n<1 is undefined */

    /* enlarge cache if necessary */
    if (n >= cached) {
        size_t new_cap = (size_t)(n + 1);           /* cache[0] unused */
        cache = seq_realloc(cache, cached, new_cap);
        for (idx_t i = cached; i <= n; ++i)         /* initialise new slots */
            cache[i] = 0;
        cached = new_cap;
    }

    /* initialise base values (only once) */
    if (n >= 1 && cache[1] == 0) cache[1] = (seq_t)853;
    if (n >= 2 && cache[2] == 0) cache[2] = (seq_t)1075;
    if (n >= 3 && cache[3] == 0) cache[3] = (seq_t)1413;
    if (n >= 4 && cache[4] == 0) cache[4] = (seq_t)1987;
    if (n >= 5 && cache[5] == 0) cache[5] = (seq_t)2925;

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)3 * cache[i-1]
                       - (seq_t)5 * cache[i-3]
                       + cache[i-4]
                       + (seq_t)2 * cache[i-5];
        }
    }

    return cache[n];
}
