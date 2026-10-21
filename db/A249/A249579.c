/*
 * A249579  List of quadruples (r,s,t,u): the matrix M =
 *          [[4,12,9][2,5,3][1,2,1]] is raised to successive powers, then
 *          (r,s,t,u) are the square roots of M[3,1], M[3,3], M[1,1], M[1,3]
 *          respectively.
 * https://oeis.org/A249579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 77, u64 n <= 152,
 *            u128 n <= 301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-4)+a(n-8) for n ≥ 8
   Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=0,
                    a(4)=1, a(5)=1, a(6)=2, a(7)=3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of elements currently stored */

    /* Extend cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – shouldn't happen */
        cache = tmp;

        /* Initialise new entries using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)   cache[i] = (seq_t)0;
            else if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)1;
            else if (i == 3) cache[i] = (seq_t)0;
            else if (i == 4) cache[i] = (seq_t)1;
            else if (i == 5) cache[i] = (seq_t)1;
            else if (i == 6) cache[i] = (seq_t)2;
            else if (i == 7) cache[i] = (seq_t)3;
            else            cache[i] = cache[i-4] * (seq_t)3 + cache[i-8];
        }
        cache_len = new_len;
    }
    return cache[n];
}
