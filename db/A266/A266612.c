/*
 * A266612  Binary representation of the middle column of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=101; for n>=3,
 *            a(n)=10*a(n-1)+((n==3)?0:(((n-4)&1)?1:0))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised term array */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t sz     = 0;      /* number of elements stored */

    /* initialise storage if needed */
    if (!cache) {
        cap   = 4;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        sz    = 0;
    }

    /* grow allocated array if required */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all terms from sz up to n */
    for (idx_t i = sz; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)10;
        } else if (i == 2) {
            cache[i] = (seq_t)101;
        } else {
            seq_t prev = cache[i-1];
            seq_t digit = (i == 3) ? (seq_t)0
                                  : (((i-4) & 1) ? (seq_t)1 : (seq_t)0);
            cache[i] = prev * (seq_t)10 + digit;
        }
    }

    sz = n + 1;
    return cache[n];
}
