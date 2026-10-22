/*
 * A321573  Row sums of A321624.
 * https://oeis.org/A321573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2); a(0)=1, a(1)=2, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    static seq_t *cache = NULL;
    static idx_t cached = 0;

    if (cache == NULL) {
        size_t init_cap = 3;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cached = 3;
    }

    if (n >= cached) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cached; i <= n; ++i) {
            seq_t term1 = cache[i - 1] * (seq_t)2;
            seq_t term2 = cache[i - 2] * (seq_t)3;
            cache[i] = term1 + term2;
        }
        cached = new_cap;
    }

    return cache[n];
}
