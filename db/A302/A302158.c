/*
 * A302158  Number of nX3 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A302158
   Empirical recurrence for n > 11:
       a(n) = a(n-1) + 8*a(n-3) - 6*a(n-4) - 4*a(n-6) + 4*a(n-7)
   Initial values (1‑based):
       a(1)= 3, a(2)= 9, a(3)= 7, a(4)=17, a(5)=31, a(6)=57, a(7)=111,
       a(8)=265, a(9)=527, a(10)=1065, a(11)=2431
   The function caches computed values so calls out of order are handled.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behavior below offset is irrelevant */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;         /* highest index filled in cache */

    /* grow cache if necessary */
    if (n > sz) {
        size_t need = (size_t)n + 1;          /* +1 because we ignore index 0 */
        if (need > cap) {
            size_t newcap = (cap == 0) ? need : cap;
            while (newcap < need) newcap <<= 1;
            seq_t *newcache = (seq_t*)malloc(newcap * sizeof(seq_t));
            if (cache) {
                for (size_t i = 1; i <= sz; ++i) newcache[i] = cache[i];
                free(cache);
            }
            cache = newcache;
            cap   = newcap;
        }

        /* fill missing entries up to n */
        static const seq_t init_vals[12] = {
            0,      /* unused 0th entry */
            3, 9, 7, 17, 31, 57, 111, 265, 527, 1065, 2431
        };

        for (idx_t i = sz + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i-1] + (seq_t)8 * cache[i-3]
                           - (seq_t)6 * cache[i-4]
                           - (seq_t)4 * cache[i-6]
                           + (seq_t)4 * cache[i-7];
            }
        }
        sz = n;
    }
    return cache[n];
}
