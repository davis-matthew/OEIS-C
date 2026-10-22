/*
 * A397835  Number of ternary words of length n over {a,b,c} containing the
 *          consecutive word abcabc.
 * https://oeis.org/A397835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 9*a(n-2) - a(n-3) + 6*a(n-4)
   - 9*a(n-5) - a(n-6) + 3*a(n-7);
   Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1,a(7)=6. */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Static cache for computed values */
static seq_t *cache = NULL;    /* array of a(k) for k <= computed */
static size_t cap = 0;         /* allocated size (number of elements) */
static size_t computed = 0;    /* highest index for which a(index) is known */

/* Resize the cache if necessary and copy old values */
static void grow_cache(size_t new_cap) {
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (cache) {
        size_t copy_len = (cap < new_cap) ? cap : new_cap;
        for (size_t i = 0; i < copy_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
    }
    cache = new_cache;
    cap = new_cap;
}

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n) {
    if ((size_t)n + 1 > cap) {
        grow_cache((size_t)n + 1);
    }
}

/* Main function computing A397835 */
seq_t a(idx_t n) {
    /* Return known small values directly */
    if (n < 6) return (seq_t)0;
    if (n == 6) return (seq_t)1;
    if (n == 7) return (seq_t)6;

    /* Ensure the cache is large enough for n */
    ensure_cache(n);

    /* Compute missing values up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        if (i <= 5) {
            cache[i] = (seq_t)0;
        } else if (i == 6) {
            cache[i] = (seq_t)1;
        } else if (i == 7) {
            cache[i] = (seq_t)6;
        } else {
            /* recurrence uses 7 previous terms */
            cache[i] =  6 * cache[i-1]
                       - 9 * cache[i-2]
                       -     cache[i-3]
                       + 6 * cache[i-4]
                       - 9 * cache[i-5]
                       -     cache[i-6]
                       + 3 * cache[i-7];
        }
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
