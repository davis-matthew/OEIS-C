/*
 * A302556  Expansion of x*(1 + 2*x*(5 - 4*x)*(1 + x^2)*(1 + x^4))/((1 - x)*(1
 *          - 10*x^9)).
 * https://oeis.org/A302556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-9) - 10*a(n-10) for n≥10 with initial
 *            terms a(0)=0, a(1)=1, a(2)=11, a(3)=3, a(4)=13, a(5)=5, a(6)=15,
 *            a(7)=7, a(8)=17, a(9)=9
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    static const seq_t base[10] = {
        (seq_t)0, (seq_t)1, (seq_t)11,
        (seq_t)3, (seq_t)13, (seq_t)5,
        (seq_t)15, (seq_t)7, (seq_t)17,
        (seq_t)9
    };

    if ((size_t)n >= cache_size) {
        size_t need = (size_t)n + 1;
        if (cache_cap < need) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i < 10) {
                cache[i] = base[i];
            } else {
                cache[i] = cache[i-1]
                         + (seq_t)10 * cache[i-9]
                         - (seq_t)10 * cache[i-10];
            }
        }
        cache_size = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
