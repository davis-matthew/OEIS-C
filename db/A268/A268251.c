/*
 * A268251  Expansion of x*(1 + 2*x - 96*x^2 - 148*x^3 + 45*x^4 + 50*x^5 +
 *          2*x^6)/(1 - 99*x^2 + 99*x^4 - x^6).
 * https://oeis.org/A268251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-2) - 99*a(n-4) + a(n-6) for n > 7 initial values:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=50, a(5)=243, a(6)=4802,
 *            a(7)=23763
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[8] = {0, 1, 2, 3, 50, 243, 4802, 23763};

    if (n < 8)
        return init[n];

    /* static cache */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static idx_t  size    = 0;

    if (cache == NULL) {
        /* allocate space for the first 8 terms */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        size = 8;
    }

    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t t1 = cache[i - 2];
        seq_t t2 = cache[i - 4];
        seq_t t3 = cache[i - 6];
        seq_t val = 99 * t1 - 99 * t2 + t3;
        cache[i] = val;
    }
    size = n + 1;
    return cache[n];
}
