/*
 * A368760  a(n) = n! * (1 + Sum_{k=0..n} k^3 / k!).
 * https://oeis.org/A368760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n^3, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t cache_len = 0;          /* number of values cached */

    if (n >= cache_len) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely in tests */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                seq_t term = (seq_t)i * cache[i - 1]
                           + (seq_t)i * (seq_t)i * (seq_t)i;
                cache[i] = term;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
