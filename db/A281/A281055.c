/*
 * A281055  Number of nX7 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -6*a(n-2) -4*a(n-3) +5*a(n-4) +9*a(n-5)
 *            -3*a(n-6) -3*a(n-7) -3*a(n-8) -9*a(n-9) +6*a(n-11) +8*a(n-12)
 *            +2*a(n-13) +3*a(n-14) -3*a(n-15) -3*a(n-16) -3*a(n-17) -a(n-18)
 *            -a(n-19) for n>23; initial values: a(1)=13, a(2)=535, a(3)=2828,
 *            a(4)=10570, a(5)=32370, a(6)=87995, a(7)=221212, a...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 56, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static inline seq_t mul_by_int(seq_t a, unsigned int m)
{
    return a * (seq_t)m;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_computed = 0;

    /* first 24 terms */
    static const seq_t init_values[25] = {
        0ULL,
        13ULL, 535ULL, 2828ULL, 10570ULL, 32370ULL, 87995ULL,
        221212ULL, 526340ULL, 1201939ULL, 2661939ULL, 5752796ULL,
        12190173ULL, 25413086ULL, 52258597ULL, 106216045ULL,
        213723050ULL, 426288170ULL, 843735147ULL,
        1658596579ULL, 3240610144ULL, 6297012052ULL,
        12175728717ULL, 23437264280ULL, 44930659786ULL
    };

    if (n == 0) return 0;   /* safety, not used by OEIS offset */

    size_t need = (size_t)n + 1;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 1; i <= 24 && i < cache_cap; ++i)
            cache[i] = init_values[i];
    }

    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n <= last_computed) return cache[(size_t)n];

    for (size_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 24) {
            cache[i] = init_values[i];
            continue;
        }

        seq_t pos = 0, neg = 0;
        /* positive contributions */
        pos += mul_by_int(cache[i-1], 5);
        pos += mul_by_int(cache[i-4], 5);
        pos += mul_by_int(cache[i-5], 9);
        pos += mul_by_int(cache[i-11], 6);
        pos += mul_by_int(cache[i-12], 8);
        pos += mul_by_int(cache[i-13], 2);
        pos += mul_by_int(cache[i-14], 3);
        /* negative contributions */
        neg += mul_by_int(cache[i-2], 6);
        neg += mul_by_int(cache[i-3], 4);
        neg += mul_by_int(cache[i-6], 3);
        neg += mul_by_int(cache[i-7], 3);
        neg += mul_by_int(cache[i-8], 3);
        neg += mul_by_int(cache[i-9], 9);
        neg += mul_by_int(cache[i-15], 3);
        neg += mul_by_int(cache[i-16], 3);
        neg += mul_by_int(cache[i-17], 3);
        neg += mul_by_int(cache[i-18], 1);
        neg += mul_by_int(cache[i-19], 1);

        cache[i] = pos - neg;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
