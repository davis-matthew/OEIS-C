/*
 * A287830  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 7.
 * https://oeis.org/A287830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 4*a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached results – allows O(n) computation in any call order. */
    static seq_t *cache = NULL;     /* Array of computed terms. */
    static idx_t  capacity = 0;     /* Size of the allocated array. */
    static idx_t  computed = 0;     /* Number of terms already stored. */

    /* If we already know the value, just return it. */
    if (n < computed)
        return cache[n];

    /* Ensure the array is large enough to hold indices 0..n. */
    if (!cache || capacity <= n) {
        idx_t newcap = n + 1;
        seq_t *tmp;

#ifdef __cplusplus
        /* In C++ use operator new to construct objects. */
        tmp = new seq_t[newcap];
#else
        /* In C allocate raw memory – trivial types only need zeroing. */
        tmp = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
#endif
        if (!tmp)                     /* Allocation failure – abort */
            return (seq_t)0;

        /* Preserve already computed values. */
        if (cache) {
            for (idx_t i = 0; i < computed; ++i)
                tmp[i] = cache[i];
            /* Free the old storage. */
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }
        cache    = tmp;
        capacity = newcap;
    }

    /* Compute any missing terms. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)10;
        } else {
            cache[i] = 9 * cache[i - 1] + 4 * cache[i - 2];
        }
    }
    computed = n + 1;

    return cache[n];
}
