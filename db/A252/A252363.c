/*
 * A252363  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3) + 5*a(n-4) + a(n-5) -
 *            2*a(n-6) - 2*a(n-7) - a(n-8) for n>9 Initial values (OEIS offset
 *            1): a(1)=152, a(2)=276, a(3)=834, a(4)=2338, a(5)=6298,
 *            a(6)=17340, a(7)=48034, a(8)=132168, a(9)=363764
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of range for this sequence */

    static seq_t *cache = NULL;       /* cache[0] unused, 1… */
    static size_t cache_cap = 0;
    static size_t cache_size = 0;     /* highest index computed so far */

    /* ensure we have enough space for indices 0..n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed: return 0 (should not happen) */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* static table of the first nine terms */
    static const seq_t init[9] = {
        152ULL, 276ULL, 834ULL, 2338ULL, 6298ULL,
        17340ULL, 48034ULL, 132168ULL, 363764ULL
    };

    /* compute terms up to n if necessary */
    size_t i;
    for (i = cache_size + 1; i <= (size_t)n; ++i) {
        if (i <= 9) {
            cache[i] = init[i - 1];
        } else { /* use the empirical recurrence */
            seq_t val = cache[i - 1];
            val += (seq_t)2 * cache[i - 2];
            val += (seq_t)6 * cache[i - 3];
            val += (seq_t)5 * cache[i - 4];
            val += cache[i - 5];
            val -= (seq_t)2 * cache[i - 6];
            val -= (seq_t)2 * cache[i - 7];
            val -= cache[i - 8];
            cache[i] = val;
        }
    }
    cache_size = (size_t)n;

    return cache[n];
}
