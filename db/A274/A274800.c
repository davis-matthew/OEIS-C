/*
 * A274800  Number of n X 5 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) (0,-1) or (-1,0) and new values introduced
 *          in order 0..2.
 * https://oeis.org/A274800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 35*a(n-2) + 42*a(n-3) - 41*a(n-4) - 60*a(n-5)
 *            - 23*a(n-6) - 2*a(n-7) Initial values: a(1)=8, a(2)=60,
 *            a(3)=420, a(4)=3104, a(5)=22988, a(6)=169328, a(7)=1252608 All
 *            arithmetic uses the seq_t type, which may be an unsigned integer
 *            type or an arbitrary‑precision integer class. The...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* abort, realloc */


seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* value below the offset */

    /* static cache of computed terms, 1‑based values stored in 0‑based array */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initial values for n = 1..7 */
    static const seq_t init_vals[7] = {
        (seq_t)8,
        (seq_t)60,
        (seq_t)420,
        (seq_t)3104,
        (seq_t)22988,
        (seq_t)169328,
        (seq_t)1252608
    };

    /* grow cache if necessary */
    if (n > cache_len) {
        idx_t new_cap = n;
        if (new_cap > cache_cap) {
#ifdef __cplusplus
            /* C++: allocate new array with custom constructor */
            seq_t *new_cache = new seq_t[new_cap];
            if (cache) {
                for (idx_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                delete [] cache;
            }
#else
            /* C: realloc works because seq_t is a built–in type
               and we overwrite all elements before first use.   */
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)
                abort();
#endif
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* compute and store new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 7) {
                val = init_vals[i - 1];
            } else {
                val = 2        * cache[i - 2]
                    + 35      * cache[i - 3]
                    + 42      * cache[i - 4]
                    - 41      * cache[i - 5]
                    - 60      * cache[i - 6]
                    - 23      * cache[i - 7]
                    - 2       * cache[i - 8];
            }
            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
