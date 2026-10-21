/*
 * A275831  a(n) = (sqrt(7)*csc(Pi/7)/2)^n + (-sqrt(7)*csc(2*Pi/7)/2)^n +
 *          (-sqrt(7)*csc(4*Pi/7)/2)^n.
 * https://oeis.org/A275831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) + 7*a(n-3) (n > 2) Initial values: a(0)=3,
 *            a(1)=0, a(2)=14
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
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
    static size_t cap   = 0;   /* current capacity of cache */

    /* ensure the cache can hold index n */
    if ((size_t)n + 1 > cap) {
        /* grow cache : at least double, but at least (n+1) */
        size_t new_cap = (n + 1 > 2 * cap) ? (n + 1) : 2 * cap;
        if (new_cap < 3) new_cap = 3;          /* need space for 0,1,2 */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed, return 0 */
        cache = tmp;

        /* compute the newly added entries */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)3;
            } else if (i == 1) {
                cache[i] = (seq_t)0;
            } else if (i == 2) {
                cache[i] = (seq_t)14;
            } else {
                cache[i] = (seq_t)7 * cache[i - 2] + (seq_t)7 * cache[i - 3];
            }
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
