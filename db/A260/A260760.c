/*
 * A260760  Number of (n+2)X(1+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00010011.
 * https://oeis.org/A260760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3)+3*a(n-4)+2*a(n-5)+5*a(n-6)
 *            +5*a(n-8)+2*a(n-9)+3*a(n-10)+4*a(n-11)
 *            +2*a(n-12)+6*a(n-13)+a(n-14) (n>15) Initial values: a(1)=48,
 *            a(2)=82, a(3)=190, a(4)=350, a(5)=622, a(6)=1315, a(7)=2478,
 *            a(8)=4597, a(9)=9184, a(10)=17591, a(11)=33096, a(12)=64829,
 *            a(13)=124590, a(14)=236647, a(15...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 62, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure capacity for indices up to n */
    if (cache_cap <= (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* Allocation failed – abort gracefully */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        /* Initialize new elements to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Initial values for n <= 15 */
    static const seq_t init_vals[16] = {
        0ULL,           /* dummy for index 0 */
        48ULL, 82ULL, 190ULL, 350ULL, 622ULL,
        1315ULL, 2478ULL, 4597ULL, 9184ULL, 17591ULL,
        33096ULL, 64829ULL, 124590ULL, 236647ULL, 458914ULL
    };

    size_t start = cache_len + 1;
    for (idx_t i = start; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t val;
        if (i <= 15) {
            val = init_vals[idx];
        } else {
            val = (seq_t)4 * cache[idx - 3];
            val += (seq_t)3 * cache[idx - 4];
            val += (seq_t)2 * cache[idx - 5];
            val += (seq_t)5 * cache[idx - 6];
            val += (seq_t)5 * cache[idx - 8];
            val += (seq_t)2 * cache[idx - 9];
            val += (seq_t)3 * cache[idx - 10];
            val += (seq_t)4 * cache[idx - 11];
            val += (seq_t)2 * cache[idx - 12];
            val += (seq_t)6 * cache[idx - 13];
            val += (seq_t)1 * cache[idx - 14];
        }
        cache[idx] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
