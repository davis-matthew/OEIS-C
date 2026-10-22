/*
 * A301286  Partial sums of A301285.
 * https://oeis.org/A301286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 5*a(n-3) - 5*a(n-4) + 5*a(n-5) -
 *            5*a(n-6) + 3*a(n-7) - 2*a(n-8) + a(n-9) (initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=20, a(4)=32, a(5)=46, a(6)=66, a(7)=89,
 *            a(8)=111)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* base values for n = 0 … 8 */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)4, (seq_t)10, (seq_t)20, (seq_t)32,
        (seq_t)46, (seq_t)66, (seq_t)89, (seq_t)111
    };

    /* we need at least n+1 entries in the cache */
    idx_t needed = (n > 8 ? n + 1 : 9);
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed – undefined */
        cache = new_cache;

        /* initialise new entries */
        for (idx_t i = cache_size; i < needed; ++i) {
            if (i < 9) {
                cache[i] = base[i];
            } else {
                /* compute using the recurrence */
                cache[i] = (seq_t)2 * cache[i - 1]
                         - (seq_t)3 * cache[i - 2]
                         + (seq_t)5 * cache[i - 3]
                         - (seq_t)5 * cache[i - 4]
                         + (seq_t)5 * cache[i - 5]
                         - (seq_t)5 * cache[i - 6]
                         + (seq_t)3 * cache[i - 7]
                         - (seq_t)2 * cache[i - 8]
                         + cache[i - 9];
            }
        }
        cache_size = needed;
    }
    return cache[n];
}
