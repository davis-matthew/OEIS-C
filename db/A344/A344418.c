/*
 * A344418  a(n) = n*a(n-1) + n^(1+n mod 2), a(0) = 0.
 * https://oeis.org/A344418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = n*a(n-1) + (n if n is even, 1 if n is odd) for n
 *            >= 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* for seq_alloc_new_array() */
#include "../utils/seq_free_array.h"       /* for seq_free_array() */

static seq_t *cache = NULL;   /* stores a[0..computed-1] */
static idx_t computed = 0;    /* number of terms currently stored */
static idx_t cache_cap = 0;   /* allocated capacity */

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* a(0) = 0 */

    /* Ensure the cache is large enough */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = 0;
        computed = 1;
    } else if (cache_cap <= n) {
        /* Allocate a larger array and copy old values */
        idx_t new_cap = n + 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < computed; ++i) {
            new_arr[i] = cache[i];
        }
        seq_free_array(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val = ((seq_t)i) * cache[i - 1];   /* i * a(i-1) */
        if (i & 1)
            val += 1;      /* i is odd -> add 1 */
        else
            val += i;      /* i is even -> add i */
        cache[i] = val;
    }
    computed = n + 1;
    return cache[n];
}
