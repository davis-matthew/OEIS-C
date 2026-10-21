/*
 * A267082  Coefficient of x^4 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=6, a(2)=456, a(3)=6240, a(4)=131238, a(5)=2238780,
 *            a(6)=41011296, a(7)=730283034 For n>7: a(n) = 14*a(n-1) +
 *            90*a(n-2) - 350*a(n-3) + 90*a(n-4) + 14*a(n-5) - a(n-6);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of elements allocated in cache */
    static idx_t cached_upto  = 0;   /* highest index that has been computed */

    /* Initialise cache with the first 8 terms */
    if (cache == NULL) {
        /* Size for the first 8 terms */
        const idx_t init_size = 8;
        /* Allocate raw memory */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        /* Construct the objects if needed (C++ only) */
        for (idx_t i = 0; i < init_size; ++i) {
#if defined(__cplusplus)
            new (&cache[i]) seq_t();
#endif
        }

        /* Assign the known initial values */
        cache[0] = 0;
        cache[1] = 6;
        cache[2] = 456;
        cache[3] = 6240;
        cache[4] = 131238;
        cache[5] = 2238780;
        cache[6] = 41011296;
        cache[7] = 730283034;

        cached_size = init_size;
        cached_upto  = 7;
    }

    /* If the requested term is already computed, return it */
    if (n <= cached_upto)
        return cache[n];

    /* Grow the cache if necessary */
    idx_t new_size = n + 1;
    if (new_size > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */

        /* Construct any new objects (C++ only) */
        for (idx_t i = cached_size; i < new_size; ++i) {
#if defined(__cplusplus)
            new (&tmp[i]) seq_t();
#endif
        }

        cache = tmp;
        cached_size = new_size;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        seq_t t1 = 14 * cache[i-1];
        seq_t t2 = 90 * cache[i-2];
        seq_t t3 = 350 * cache[i-3];
        seq_t t4 = 90 * cache[i-4];
        seq_t t5 = 14 * cache[i-5];
        seq_t t6 = cache[i-6];

        seq_t positive = t1 + t2 + t4 + t5;
        seq_t negative = t3 + t6;
        cache[i] = positive - negative;
    }

    cached_upto = n;
    return cache[n];
}
