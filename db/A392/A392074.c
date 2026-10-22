/*
 * A392074  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *          binomial(k,4*(n-3*k)).
 * https://oeis.org/A392074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 49, u64 n <= 96, u128 n <= 187,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392074
 * a(n) = 8*a(n-3) - 24*a(n-6) + 32*a(n-9) - 16*a(n-12) + 48*a(n-13)
 * with initial values:
 * a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=0, a(5)=0, a(6)=4,
 * a(7)=0, a(8)=0, a(9)=8, a(10)=0, a(11)=0, a(12)=16, a(13)=48.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 13;
    static int init = 0;

    if (!init) {
        size_t new_cap = 16;
        cache = seq_alloc(new_cap);
        cap = new_cap;
        cache[0]  = 1;
        cache[1]  = 0;
        cache[2]  = 0;
        cache[3]  = 2;
        cache[4]  = 0;
        cache[5]  = 0;
        cache[6]  = 4;
        cache[7]  = 0;
        cache[8]  = 0;
        cache[9]  = 8;
        cache[10] = 0;
        cache[11] = 0;
        cache[12] = 16;
        cache[13] = 48;
        init = 1;
    }

    if ((idx_t)n + 1 > (idx_t)cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        cache = tmp;
        cap = new_cap;
    }

    if (n <= last) return cache[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v = cache[i-3] * (8u);
        v = v - cache[i-6] * (24u);
        v = v + cache[i-9] * (32u);
        v = v - cache[i-12] * (16u);
        v = v + cache[i-13] * (48u);
        cache[i] = v;
    }
    last = n;
    return cache[n];
}
