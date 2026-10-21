/*
 * A255353  Denominators in an expansion of 3 - sqrt(5) as a sum of fractions
 *          +-1/d.
 * https://oeis.org/A255353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 8*a(n-3) - 8*a(n-6) + a(n-9) for n > 9 with initial
 *            values a(1)=2, a(2)=3, a(3)=6, a(4)=15, a(5)=24, a(6)=40,
 *            a(7)=104, a(8)=168, a(9)=273
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the first nine terms directly */
    if (n <= 9) {
        static const seq_t init[10] = {0, 2, 3, 6, 15, 24, 40, 104, 168, 273};
        return init[n];
    }

    /* Static cache for memoization */
    static seq_t *cache = NULL;     /* cache[1..cached_n] valid */
    static idx_t cached_n = 0;      /* largest n for which cache[i] is valid */

    /* Expand cache if necessary */
    if (n > cached_n) {
        /* Allocate at least n+1 entries (index 0 unused) */
        idx_t new_cap = n + 1;
        seq_t *temp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!temp) exit(1);          /* allocation failed */
        cache = temp;

        /* Initialize first nine entries if this is the first allocation */
        if (cached_n == 0) {
            cache[1] = 2;
            cache[2] = 3;
            cache[3] = 6;
            cache[4] = 15;
            cache[5] = 24;
            cache[6] = 40;
            cache[7] = 104;
            cache[8] = 168;
            cache[9] = 273;
            cached_n = 9;
        }

        /* Compute terms from cached_n+1 up to n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t val = cache[i - 3];
            val *= 8;
            val -= cache[i - 6] * 8;
            val += cache[i - 9];
            cache[i] = val;
        }
        cached_n = n;
    }

    return cache[n];
}
