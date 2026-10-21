/*
 * A288662  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 493", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=100, a(3)=11, a(4)=11000, a(5)=111,
 *            a(6)=1110000, a(n)=111*a(n-2)-1110*a(n-4)+1000*a(n-6) for n>6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  len   = 0;
    static int    init  = 0;   /* 0 = not initialised yet */

    /* ensure cache can hold indices up to n */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 8;          /* at least 8 to store 0..7 */
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first 7 values once */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)100;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)11000;
        cache[5] = (seq_t)111;
        cache[6] = (seq_t)1110000;
        len    = 7;
        init   = 1;
    }

    /* compute up to n if required */
    if (n >= len) {
        for (idx_t k = len; k <= n; ++k) {
            seq_t t1 = (seq_t)111  * cache[k - 2];
            seq_t t2 = (seq_t)1110 * cache[k - 4];
            seq_t t3 = (seq_t)1000 * cache[k - 6];
            cache[k] = t1 - t2 + t3;
        }
        len = n + 1;
    }

    return cache[n];
}
