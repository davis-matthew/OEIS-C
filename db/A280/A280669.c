/*
 * A280669  Number of n X 4 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence: A280669
   a(1)=11, a(2)=338, a(3)=10205, a(4)=310365, a(5)=9439606
   For n>5:
        a(n) = 29*a(n-1) + 44*a(n-2) - 27*a(n-3) - 81*a(n-4)  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[0] unused, cache[1]..cache[n] */
    static size_t cache_size = 0;   /* current capacity */

    if (n == 0) return 0;           /* undefined, but harmless */

    /* enlarge cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        /* Allocate and construct C++ objects */
        new_cache = new seq_t[new_size];
#else
        /* C mode: allocate raw memory */
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (!new_cache) return 0;   /* allocation failure */

        /* copy old values */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        }

        /* free old storage */
        if (cache) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }

        /* compute new entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) continue;          /* unused */
            else if (i == 1) new_cache[i] = (seq_t)11;
            else if (i == 2) new_cache[i] = (seq_t)338;
            else if (i == 3) new_cache[i] = (seq_t)10205;
            else if (i == 4) new_cache[i] = (seq_t)310365;
            else if (i == 5) new_cache[i] = (seq_t)9439606;
            else {
                /* recurrence for i >= 6 */
                seq_t t = 29 * new_cache[i - 1] + 44 * new_cache[i - 2];
                t -= 27 * new_cache[i - 3];
                t -= 81 * new_cache[i - 4];
                new_cache[i] = t;
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
