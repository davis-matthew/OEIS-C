/*
 * A289976  p-INVERT of (0,0,1,2,3,5,8,...), the Fibonacci numbers preceded by
 *          two zeros, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=5, a(6)=9; a(n) =
 *            2*a(n-1) + a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + a(n-6) for n >=
 *            7
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

seq_t a(idx_t n)
{
    /* Base values for n < 7 */
    static const seq_t base[7] = {0, 0, 1, 1, 2, 5, 9};

    if (n < 7) return base[n];

    /* Static cache for dynamic programming */
    static seq_t *cache      = NULL; /* array of computed terms */
    static size_t capacity   = 0;    /* allocated capacity */
    static idx_t  computed   = 0;    /* number of valid entries in cache */

    /* Ensure capacity for indices 0..n */
    idx_t needed = n + 1;
    if (capacity < (size_t)needed) {
        size_t newcap = capacity ? capacity * 2 : 64;
        while (newcap < (size_t)needed) newcap <<= 1;

        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[newcap];
        if (cache) {
            for (idx_t i = 0; i < computed; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
#else
        tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
#endif
        if (!tmp) exit(1);
        cache    = tmp;
        capacity = newcap;
    }

    /* Initialize base values once */
    if (computed == 0) {
        for (idx_t i = 0; i < 7; ++i) cache[i] = base[i];
        computed = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        cache[i] = 2 * a1 + a2 - a3 - 2 * a4 - a5 + a6;
    }
    computed = n + 1;

    return cache[n];
}
