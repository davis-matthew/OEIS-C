/*
 * A373961  Number of compositions of 6*n-1 into parts 5 and 6.
 * https://oeis.org/A373961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of (6*n-1) into parts 5 and 6.
 *            Recurrence: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7;
 *            a(n)=6*a(n-1)-15*a(n-2)+20*a(n-3)-15*a(n-4)+7*a(n-5)-a(n-6) for
 *            n≥7
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* 0‑based array: cache[i] = a(i+1) */
    static size_t cache_len = 0;     /* current length of cache */

    if (n == 0) return 0;          /* unused but safe for the offset definition */

    size_t target = (size_t)n;     /* desired length of cache (≥ n) */

    if (target > cache_len) {
        size_t new_len = target;
        seq_t *new_cache;
#ifdef __cplusplus
        /* C++: allocate new array and copy existing entries. */
        new_cache = new seq_t[new_len];
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
#else
        /* C: realloc the existing block. */
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
#endif
        cache = new_cache;

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) cache[i] = 1;
            else if (i == 1) cache[i] = 2;
            else if (i == 2) cache[i] = 3;
            else if (i == 3) cache[i] = 4;
            else if (i == 4) cache[i] = 5;
            else if (i == 5) cache[i] = 7;
            else
                cache[i] =
                    6 * cache[i - 1] - 15 * cache[i - 2] + 20 * cache[i - 3]
                    - 15 * cache[i - 4] + 7 * cache[i - 5] - cache[i - 6];
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
