/*
 * A301603  Number of nX3 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+3*a(n-2)+4*a(n-3)+2*a(n-4)-8*a(n-5)-17*a(n-6)-19*a(n-7)-9*a(n-8)+6*a(n-9)+21*a(n-10)+15*a(n-11)+11*a(n-12)-2*a(n-13)-2*a(n-14)-4*a(n-15)-a(n-16)-a(n-17)+a(n-18)
 *            Initial values:
 *            a(1)=1,a(2)=2,a(3)=2,a(4)=8,a(5)=13,a(6)=26,a(7)=55,a(8)=154,a(9)=356,a(10)=884,
 *            a(11)=2161,a(12)=5440,a(1...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;           /* array indexed 0..current_max */
    static size_t cache_cap = 0;          /* allocated size */
    static idx_t max_n = 0;               /* highest n computed */

    if (n == 0)                           /* undefined in OEIS */
        return 0;

    if (n > max_n) {
        /* enlarge cache if necessary */
        size_t new_cap = (size_t)(n + 1);  /* index 0 unused, so n+1 slots */
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* first 18 values (explicit definition) */
        static const seq_t init[18] = {
            1ULL, 2ULL, 2ULL, 8ULL, 13ULL, 26ULL, 55ULL, 154ULL,
            356ULL, 884ULL, 2161ULL, 5440ULL, 13511ULL, 33780ULL,
            84398ULL, 211338ULL, 528999ULL, 1325014ULL
        };

        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 18) {
                cache[i] = init[i - 1];
            } else {
                cache[i] = cache[i - 1] + 3 * cache[i - 2] + 4 * cache[i - 3] + 2 * cache[i - 4]
                            - 8 * cache[i - 5] - 17 * cache[i - 6] - 19 * cache[i - 7] - 9 * cache[i - 8]
                            + 6 * cache[i - 9] + 21 * cache[i - 10] + 15 * cache[i - 11] + 11 * cache[i - 12]
                            - 2 * cache[i - 13] - 2 * cache[i - 14] - 4 * cache[i - 15] - cache[i - 16] - cache[i - 17]
                            + cache[i - 18];
            }
        }
        max_n = n;
    }

    return cache[n];
}
