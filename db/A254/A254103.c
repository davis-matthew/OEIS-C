/*
 * A254103  Permutation of natural numbers: a(0) = 0, a(2n) = (3*a(n))-1,
 *          a(2n+1) = floor((3*(1+a(n)))/2).
 * https://oeis.org/A254103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(2n) = 3*a(n) - 1, a(2n+1) = floor(3*(1 + a(n))/2)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static bool *used  = NULL;
    static size_t cap  = 0;

    /* Ensure capacity is enough for index n */
    if ((size_t)n >= cap) {
        size_t newcap = (cap > 0) ? cap << 1 : 1;
        while (newcap <= (size_t)n) newcap <<= 1;

        seq_t *nc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        bool *nu  = (bool *)realloc(used,  newcap * sizeof(bool));
        if (!nc || !nu) {
            /* memory allocation failed – abort (unlikely in test harness) */
            abort();
        }

        /* Initialise new entries to not computed */
        for (size_t i = cap; i < newcap; ++i) nu[i] = false;

        cache = nc;
        used  = nu;
        cap   = newcap;
    }

    if (used[n]) return cache[n];

    seq_t val;
    if (n == 0) {
        val = (seq_t)0;
    } else if ((n & 1) == 0) {          /* n is even: a(2n) */
        idx_t m = n >> 1;
        val = (seq_t)3 * a(m) - (seq_t)1;
    } else {                            /* n is odd: a(2n+1) */
        idx_t m = n >> 1;
        val = ((seq_t)3 * ((seq_t)1 + a(m))) / (seq_t)2;
    }

    cache[n] = val;
    used[n]  = true;
    return val;
}
