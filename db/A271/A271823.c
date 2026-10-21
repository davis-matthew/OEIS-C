/*
 * A271823  a(n) = binomial(2*n-4,n-1)*(n+3)/n.
 * https://oeis.org/A271823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-4,n-1)*(n+3)/n (n >= 3) a(1) = 4 a(2) = 0
 *            a(3) = 2 Recurrence: for n >= 4 n*(n+2)*(n-3)*a(n) =
 *            2*(n-2)*(2*n-5)*(n+3)*a(n-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   ugcd128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ugcd128.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;        /* largest index currently cached */

    /* initialise cache with the first three terms */
    if (last < 3) {
        if (cap < 4) {
            cap = 4;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[1] = (seq_t)4u;
        cache[2] = (seq_t)0u;
        cache[3] = (seq_t)2u;
        last = 3;
    }

    if (n == 0) return 0;        /* safety, not used in OEIS */

    /* extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        unsigned __int128 prev = (unsigned __int128)cache[i - 1];

        unsigned __int128 num = (unsigned __int128)2 * (i - 2) *
                                (unsigned __int128)(2 * i - 5) *
                                (unsigned __int128)(i + 3);

        unsigned __int128 den = (unsigned __int128)i *
                                (unsigned __int128)(i + 2) *
                                (unsigned __int128)(i - 3);

        unsigned __int128 g = ugcd128(num, den);
        num /= g; den /= g;

        g = ugcd128(prev, den);
        prev /= g; den /= g;

        /* remaining division is exact */
        unsigned __int128 res = (prev * num) / den;

        cache[i] = (seq_t)res;
    }
    last = n;
    return cache[n];
}
