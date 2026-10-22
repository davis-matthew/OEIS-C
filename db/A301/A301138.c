/*
 * A301138  Coordination sequence T2 for Zeolite Code NAB.
 * https://oeis.org/A301138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301138 : Coordination sequence T2 for Zeolite Code NAB
   Recurrence (for n ≥ 11):
       a(n) = 2*a(n-3) + a(n-4) - a(n-6) - 2*a(n-7) + a(n-10)
   Initial values:
       a(0)=1, a(1)=4, a(2)=8, a(3)=20, a(4)=40, a(5)=52, a(6)=82,
       a(7)=116, a(8)=132, a(9)=184, a(10)=236
   Computation is memoised so that calls may come in any order.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* pointer to computed terms   */
    static size_t cache_size = 0;        /* number of stored terms      */

    /* allocate / grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) { /* allocation failed; abort */
            /* In a real program we would handle this properly. */
            abort();
        }
        cache = tmp;

        static const seq_t init[11] = {
            (seq_t)1,  /* a(0) */
            (seq_t)4,  /* a(1) */
            (seq_t)8,  /* a(2) */
            (seq_t)20, /* a(3) */
            (seq_t)40, /* a(4) */
            (seq_t)52, /* a(5) */
            (seq_t)82, /* a(6) */
            (seq_t)116,/* a(7) */
            (seq_t)132,/* a(8) */
            (seq_t)184,/* a(9) */
            (seq_t)236 /* a(10) */
        };

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(n) = 2*a(n-3) + a(n-4) - a(n-6) - 2*a(n-7) + a(n-10) */
                seq_t val = 0;
                val += (seq_t)2 * cache[i - 3];
                val += cache[i - 4];
                val -= cache[i - 6];
                val -= (seq_t)2 * cache[i - 7];
                val += cache[i - 10];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
