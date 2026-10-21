/*
 * A296322  Number of n X 3 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A296322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) - 2*a(n-4) + 4*a(n-5) -
 *            3*a(n-6) + 2*a(n-7) - a(n-8); for n ≥ 9 Initial values: a(1)=1,
 *            a(2)=5, a(3)=11, a(4)=23, a(5)=54, a(6)=122, a(7)=278, a(8)=634
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[i] holds a(i+1) for i>=0  (i.e., cache[0] == a(1)) */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* memory allocated in elements of cache   */
    static idx_t max_n = 0;   /* largest n whose value is already cached */

    if (n < 1)                  /* undefined for n<1, return 0 */
        return 0;

    /* enlarge cache if necessary to store up to index n-1 */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n + 1;   /* +1 to allow indices 0..n-1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)               /* allocation failed – return 0 for safety */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* compute any terms up to n that are missing */
    if (max_n < n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            const size_t idx = (size_t)(i - 1);   /* array index for a(i) */

            /* base values for i ≤ 8 */
            if (i == 1)      cache[idx] = (seq_t)1;
            else if (i == 2) cache[idx] = (seq_t)5;
            else if (i == 3) cache[idx] = (seq_t)11;
            else if (i == 4) cache[idx] = (seq_t)23;
            else if (i == 5) cache[idx] = (seq_t)54;
            else if (i == 6) cache[idx] = (seq_t)122;
            else if (i == 7) cache[idx] = (seq_t)278;
            else if (i == 8) cache[idx] = (seq_t)634;
            else {
                /* recurrence for i ≥ 9: use only additions and a single
                   subtraction to avoid negative intermediates              */
                seq_t pos = 2 * cache[idx - 1]        /* 2*a(i-1)      */
                          + 4 * cache[idx - 3]        /* 4*a(i-3)      */
                          + 4 * cache[idx - 5]        /* 4*a(i-5)      */
                          + 2 * cache[idx - 7];       /* 2*a(i-7)      */

                seq_t neg =     cache[idx - 2]      /*   a(i-2)      */
                           + 2 * cache[idx - 4]      /*   2*a(i-4)    */
                           + 3 * cache[idx - 6]      /*   3*a(i-6)    */
                           +     cache[idx - 8];     /*   a(i-8)      */

                cache[idx] = pos - neg;
            }
        }
        max_n = n;
    }

    return cache[(size_t)(n - 1)];
}
