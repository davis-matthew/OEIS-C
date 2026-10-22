/*
 * A329470  a(n) = 2 a(n-1)^2 + 1 for n >=2 , where a(0) = 1, a(1) = 1.
 * https://oeis.org/A329470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n)=2*a(n-1)^2+1 for n>=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;
    static int init = 0;

    if (n == 0 || n == 1)
        return (seq_t)1;

    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    if (!init) {
        if (cache_cap >= 1) cache[0] = (seq_t)1;
        if (cache_cap >= 2) cache[1] = (seq_t)1;
        init = 1;
        max_computed = 1;
    }

    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i >= 2) {
                seq_t tmp = cache[i - 1];
                tmp *= tmp;
                tmp *= (seq_t)2;
                tmp += (seq_t)1;
                cache[i] = tmp;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
