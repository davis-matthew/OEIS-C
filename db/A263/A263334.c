/*
 * A263334  Number of (n+2) X (1+2) 0..2 arrays with each row and column
 *          divisible by 13, read as a base-3 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3) - 3*a(n-4); a(1)=3, a(2)=7, a(3)=19,
 *            a(4)=57
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n <= 4) {
        static const seq_t base[5] = {(seq_t)0,
                                      (seq_t)3,
                                      (seq_t)7,
                                      (seq_t)19,
                                      (seq_t)57};
        return base[n];
    }

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    if (cache.size() < 5) {
        cache.resize(5);
        cache[0] = 0;
        cache[1] = 3;
        cache[2] = 7;
        cache[3] = 19;
        cache[4] = 57;
    }
    while (cache.size() <= n) {
        seq_t val = (cache[cache.size() - 1] * 3) + cache[cache.size() - 3]
                    - (cache[cache.size() - 4] * 3);
        cache.push_back(val);
    }
    return cache[n];
#else
    /* C implementation using malloc / realloc */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0; /* allocation failure */

        /* Initialise the first five terms if needed */
        if (cache_size < 5) {
            tmp[0] = 0;
            tmp[1] = 3;
            tmp[2] = 7;
            tmp[3] = 19;
            tmp[4] = 57;
            cache_size = 5;
        }

        /* Compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = (tmp[i - 1] * 3) + tmp[i - 3]
                        - (tmp[i - 4] * 3);
            tmp[i] = val;
        }
        cache = tmp;
        cache_size = new_size;
    }
    return cache[n];
#endif
}
