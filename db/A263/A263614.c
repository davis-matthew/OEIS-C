/*
 * A263614  a(2n) = A000125(n), a(2n+1) = 2*a(2n).
 * https://oeis.org/A263614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-2)-6*a(n-4)+4*a(n-6)-a(n-8) for n>7 Base values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=2, a(5)=4, a(6)=4, a(7)=8
 * Range:     u8 n <= 20, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        if (new_size < 8) new_size = 8;

        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        }

        static const seq_t base[8] = {0, 0, 1, 2, 2, 4, 4, 8};

        for (idx_t i = cache_size; i < (idx_t)new_size; ++i) {
            if (i < 8) {
                cache[i] = base[i];
            } else {
                seq_t term1 = cache[i - 2] * (seq_t)4;
                seq_t term2 = cache[i - 4] * (seq_t)6;
                seq_t term3 = cache[i - 6] * (seq_t)4;
                seq_t term4 = cache[i - 8];
                cache[i] = term1 - term2 + term3 - term4;
            }
        }
        cache_size = (idx_t)new_size;
    }

    return cache[n];
}
