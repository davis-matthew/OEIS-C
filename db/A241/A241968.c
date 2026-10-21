/*
 * A241968  Number of length 5+3 0..n arrays with no consecutive four elements
 *          summing to more than 2*n.
 * https://oeis.org/A241968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) (n>9);
 *            a(1)=97, a(2)=1895, a(3)=16649, a(4)=92037, a(5)=376559,
 *            a(6)=1247602, a(7)=3536286, a(8)=8889273, a(9)=20314789
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_alloc_new_array.h"   /* allocates a seq_t array */
#include "../utils/free_seq_array.h"        /* frees a seq_t array */

seq_t a(idx_t n)
{
    /* static cache for computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity in terms of indices */
    static idx_t max_computed = 0;

    /* ensure capacity (add a few extra slots to reduce reallocations) */
    if (cache_cap < n + 10) {
        seq_t *new_cache = seq_alloc_new_array((size_t)(n + 10));
        if (cache != NULL) {
            /* copy existing computed values */
            for (idx_t i = 1; i <= max_computed; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = n + 10;
    }

    /* pre‑defined first nine values */
    static const seq_t init_vals[9] = {
            (seq_t)97,
            (seq_t)1895,
            (seq_t)16649,
            (seq_t)92037,
            (seq_t)376559,
            (seq_t)1247602,
            (seq_t)3536286,
            (seq_t)8889273,
            (seq_t)20314789
    };

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t t1 = 9   * cache[i - 1];
            seq_t t2 = 36  * cache[i - 2];
            seq_t t3 = 84  * cache[i - 3];
            seq_t t4 = 126 * cache[i - 4];
            seq_t t5 = 126 * cache[i - 5];
            seq_t t6 = 84  * cache[i - 6];
            seq_t t7 = 36  * cache[i - 7];
            seq_t t8 = 9   * cache[i - 8];
            cache[i] = t1 - t2 + t3 - t4 + t5 - t6 + t7 - t8 + cache[i - 9];
        }
    }
    max_computed = n;

    return cache[n];
}
