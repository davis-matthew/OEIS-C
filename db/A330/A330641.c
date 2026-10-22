/*
 * A330641  a(n) is the number of partitions of n with Durfee square of size
 *          <= 3.
 * https://oeis.org/A330641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 453, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - 3*a(n-4) + 6*a(n-6)
          - 3*a(n-8) - 2*a(n-9) + a(n-10) + 2*a(n-11) - a(n-12)
          for n > 12;
   Initial values:
   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=11,
   a(7)=15, a(8)=22, a(9)=30, a(10)=42, a(11)=56, a(12)=77.
*/

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t  cap    = 0;
static idx_t   computed = 0;

/* Initial 13 values */
static const seq_t init_vals[13] = {
    1, 1, 2, 3, 5,
    7, 11, 15, 22, 30,
    42, 56, 77
};

/* Ensure cache can hold indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[16];
#else
        cache = (seq_t*)malloc(16 * sizeof(seq_t));
#endif
        if (cache == NULL) exit(1);
        cap = 16;
    }
    if (need > cap) {
        size_t old_cap = cap;
        while (need > cap) cap <<= 1;
#ifdef __cplusplus
        seq_t *newp = new seq_t[cap];
#else
        seq_t *newp = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        if (newp == NULL) exit(1);
        for (size_t i = 0; i < computed; ++i) newp[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = newp;
    }
}

/* OEIS sequence A330641 */
seq_t a(idx_t n)
{
    ensure_size(n);
    if (n < computed) {
        return cache[(size_t)n];
    }

    /* Compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (i < 13) {
            cache[idx] = init_vals[(size_t)i];
        } else {
            seq_t val =
                2 * cache[idx-1] + cache[idx-2]
                - 2 * cache[idx-3] - 3 * cache[idx-4]
                + 6 * cache[idx-6] - 3 * cache[idx-8]
                - 2 * cache[idx-9] + cache[idx-10]
                + 2 * cache[idx-11] - cache[idx-12];
            cache[idx] = val;
        }
    }
    computed = n + 1;
    return cache[(size_t)n];
}
