/*
 * A315644  Coordination sequence Gal.6.325.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10), a(0) = 1,
 *            a(1) = 6, a(2) = 12, a(3) = 16, a(4) = 22, a(5) = 26, a(6) = 30,
 *            a(7) = 36, a(8) = 40, a(9) = 46, a(10) = 52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 11 values, 0 ≤ n ≤ 10 */
    static const seq_t init[11] = {
        1, 6, 12, 16, 22, 26, 30, 36, 40, 46, 52
    };

    /* static cache and bookkeeping */
    static seq_t *cache = NULL;      /* allocated array */
    static size_t cap    = 0;        /* current allocation size */
    static size_t csize  = 0;        /* number of values currently computed (csize-1 = last index) */

    /* immediate return for the initial segment */
    if (n <= 10) return init[(size_t)n];

    /* first allocation if necessary */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        if (cap < 11) cap = 11;          /* never smaller than 11 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failed – unlikely in tests */
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        csize = 11;                     /* values up to index 10 are known */
    } else if ((size_t)n + 1 > cap) {    /* grow capacity if needed */
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to n */
    if ((size_t)n + 1 > csize) {
        size_t start = csize;            /* first index to compute */
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t p = 0, m = 0;
            /* positive contributions */
            p += cache[i - 1] * 2;
            p += cache[i - 3] * 2;
            p += cache[i - 5] * 2;
            p += cache[i - 7] * 2;
            p += cache[i - 9] * 2;
            /* negative contributions */
            m += cache[i - 2] * 2;
            m += cache[i - 4] * 2;
            m += cache[i - 6] * 2;
            m += cache[i - 8] * 2;
            m += cache[i -10];
            cache[i] = p - m;
        }
        csize = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
