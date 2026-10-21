/*
 * A282607  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 489", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-2) + a(n-4) - 100*a(n-6) for n > 13 Initial
 *            values: a(0)=1, a(1)=0, a(2)=111, a(3)=10, a(4)=11011,
 *            a(5)=11110, a(6)=1100111, a(7)=1011010, a(8)=100111011,
 *            a(9)=1011110, a(10)=10011100111, a(11)=101011010,
 *            a(12)=1001100111011, a(13)=11001011110
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[14] = {
    (seq_t)1ULL,
    (seq_t)0ULL,
    (seq_t)111ULL,
    (seq_t)10ULL,
    (seq_t)11011ULL,
    (seq_t)11110ULL,
    (seq_t)1100111ULL,
    (seq_t)1011010ULL,
    (seq_t)100111011ULL,
    (seq_t)1011110ULL,
    (seq_t)10011100111ULL,
    (seq_t)101011010ULL,
    (seq_t)1001100111011ULL,
    (seq_t)11001011110ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;      /* number of values currently stored */

    /* Already cached? */
    if (len > n) {
        return cache[n];
    }

    /* Expand the cache if necessary */
    size_t required = (n + 1 > 14) ? (n + 1) : 14;
    if (required > cap) {
        size_t newcap = cap ? cap * 2 : required;
        if (newcap < required) {
            newcap = required;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            abort();                /* allocation failure */
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Copy the initial values that are not yet cached */
    if (len < 14) {
        size_t i = len;
        for (; i < 14 && i <= n; ++i) {
            cache[i] = _init[i];
        }
        len = i;                   /* number of cached entries */
    }

    /* Compute the remaining terms using the recurrence */
    for (size_t i = len; i <= n; ++i) {
        if (i < 14) {              /* already set by the initial copy */
            continue;
        }
        cache[i] = (seq_t)100 * cache[i-2] + cache[i-4] - (seq_t)100 * cache[i-6];
    }

    len = n + 1;
    return cache[n];
}
