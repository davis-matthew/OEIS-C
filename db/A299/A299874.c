/*
 * A299874  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2,
 *          4, 5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=13, a(3)=20, a(4)=41, a(5)=102, a(6)=253, a(7)=632,
 *            a(8)=1563, a(9)=3896, a(10)=9791, a(11)=24572, a(12)=61631,
 *            a(13)=154742, a(14)=388577, a(15)=975708, a(16)=2450385,
 *            a(17)=6154002. For n>17:
 *            a(n)=5*a(n-1)-7*a(n-2)+7*a(n-3)-24*a(n-4)+31*a(n-5)-21*a(n-6)
 *            +51*a(n-7)-50*a(n-8)+44*a(n-...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache_capacity(idx_t new_cap) {
    if (cache_cap >= new_cap) return;
    if (cache_cap < 64) cache_cap = 64;
    while (cache_cap < new_cap) cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

static const seq_t init_vals[17] = {
    (seq_t)4, (seq_t)13, (seq_t)20, (seq_t)41, (seq_t)102,
    (seq_t)253, (seq_t)632, (seq_t)1563, (seq_t)3896,
    (seq_t)9791, (seq_t)24572, (seq_t)61631, (seq_t)154742,
    (seq_t)388577, (seq_t)975708, (seq_t)2450385, (seq_t)6154002
};

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0; /* undefined for n<1 */

    if (cache_len < n) {
        ensure_cache_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 17) {
                cache[i - 1] = init_vals[i - 1];
            } else {
                seq_t t;
                t = (seq_t)5  * cache[i - 2];
                t -= (seq_t)7  * cache[i - 3];
                t += (seq_t)7  * cache[i - 4];
                t -= (seq_t)24 * cache[i - 5];
                t += (seq_t)31 * cache[i - 6];
                t -= (seq_t)21 * cache[i - 7];
                t += (seq_t)51 * cache[i - 8];
                t -= (seq_t)50 * cache[i - 9];
                t += (seq_t)44 * cache[i - 10];
                t -= (seq_t)78 * cache[i - 11];
                t += (seq_t)34 * cache[i - 12];
                t -= (seq_t)37 * cache[i - 13];
                t += (seq_t)60 * cache[i - 14];
                t -= (seq_t)10 * cache[i - 15];
                t += (seq_t)20 * cache[i - 16];
                t -= (seq_t)24 * cache[i - 17];
                cache[i - 1] = t;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
