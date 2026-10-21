/*
 * A293665  a(0) = 1; a(n) = -Sum_{d|n} a(n-d).
 * https://oeis.org/A293665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -∑_{d|n} a(n-d)
 * Range:     i8 n <= 44, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    if ((idx_t)len <= n) {
        size_t req = (size_t)n + 1;
        if (cap < req) {
            size_t newcap = cap ? cap * 2 : 16;
            if (newcap < req) newcap = req;
            cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }
        for (idx_t k = len; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            for (idx_t i = 1; i <= k / i; ++i) {
                if (k % i == 0) {
                    sum += cache[(size_t)(k - i)];
                    idx_t j = k / i;
                    if (j != i)
                        sum += cache[(size_t)(k - j)];
                }
            }
            cache[(size_t)k] = -sum;
        }
        len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
