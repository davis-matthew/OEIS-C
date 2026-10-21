/*
 * A252187  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 1 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-3) - 14*a(n-6) + 8*a(n-9) for n>15 with initial
 *            values a(1)=2055, a(2)=2610, a(3)=3705, a(4)=5460, a(5)=8945,
 *            a(6)=13737, a(7)=20974, a(8)=34209, a(9)=53194, a(10)=82196,
 *            a(11)=134418, a(12)=210132, a(13)=325600, a(14)=533076,
 *            a(15)=835480
 * Range:     u8 none, u16 n <= 9, u32 n <= 33, u64 n <= 81, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t size = 0; /* highest index computed */

    /* Initial values up to 15. The 0th element is unused. */
    static const seq_t init[16] = {
        0ULL,
        2055ULL, 2610ULL, 3705ULL, 5460ULL, 8945ULL, 13737ULL,
        20974ULL, 34209ULL, 53194ULL, 82196ULL,
        134418ULL, 210132ULL, 325600ULL, 533076ULL, 835480ULL
    };

    /* Lazy initialization of the cache. */
    if (!cache) {
        capacity = 32;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        memset(cache, 0, capacity * sizeof(seq_t));
        size = 15;                     /* we have initial terms up to 15 */
        memcpy(&cache[1], init + 1, 15 * sizeof(seq_t));
    }

    /* Extend the cache if necessary. */
    if (size < n) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        memset(&cache[capacity], 0, (new_cap - capacity) * sizeof(seq_t));
        capacity = new_cap;

        /* Compute missing terms up to n. */
        while (size < n) {
            ++size;
            seq_t val;
            if (size <= 15) {
                val = init[size];
            } else {
                /* a(n) = 7*a(n-3) - 14*a(n-6) + 8*a(n-9) */
                val = cache[size - 3] * (seq_t)7
                    + cache[size - 9] * (seq_t)8
                    - cache[size - 6] * (seq_t)14;
            }
            cache[size] = val;
        }
    }

    return cache[n];
}
