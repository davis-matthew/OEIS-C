/*
 * A289022  Wiener index of the n-Apollonian network.
 * https://oeis.org/A289022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 174*a(n-2) + 448*a(n-3) - 29*a(n-4) * -
 *            1221*a(n-5) + 2088*a(n-6) - 4050*a(n-7) + 2916*a(n-8); * Initial
 *            values: a(1)=6, a(2)=27, a(3)=204, a(4)=1941, a(5)=19572, *
 *            a(6)=198567, a(7)=1999056, a(8)=19931337
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        /* indices 0 is unused */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)27;
        cache[3] = (seq_t)204;
        cache[4] = (seq_t)1941;
        cache[5] = (seq_t)19572;
        cache[6] = (seq_t)198567;
        cache[7] = (seq_t)1999056;
        cache[8] = (seq_t)19931337;
    }

    if (n <= cache_len) {
        return cache[n];
    }

    /* extend cache to cover n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) {
        return (seq_t)0;  /* allocation failed */
    }
    cache = tmp;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        /* Positive contributions */
        pos += (seq_t)23  * cache[i - 1];
        pos += (seq_t)448 * cache[i - 3];
        pos += (seq_t)2088 * cache[i - 6];
        pos += (seq_t)2916 * cache[i - 8];
        /* Negative contributions */
        neg += (seq_t)174  * cache[i - 2];
        neg += (seq_t)29   * cache[i - 4];
        neg += (seq_t)1221 * cache[i - 5];
        neg += (seq_t)4050 * cache[i - 7];
        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n];
}
