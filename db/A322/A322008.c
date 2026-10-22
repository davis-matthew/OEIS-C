/*
 * A322008  1/(1 - Integral_{x=0..1} x^(x^n) dx), rounded to the nearest
 *          integer.
 * https://oeis.org/A322008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-8) - 2*a(n-9) + a(n-10) for n >
 *            11 with initial values: a(0)=2, a(1)=5, a(2)=10, a(3)=17,
 *            a(4)=26, a(5)=37, a(6)=50, a(7)=65, a(8)=82, a(9)=101,
 *            a(10)=123, a(11)=146
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t cache[20001];          /* enough for n up to about 10000 */
    static idx_t cache_len = 0;
    static const seq_t init[12] = {
        (seq_t)2, (seq_t)5, (seq_t)10, (seq_t)17,
        (seq_t)26, (seq_t)37, (seq_t)50, (seq_t)65,
        (seq_t)82, (seq_t)101, (seq_t)123, (seq_t)146
    };

    if (cache_len == 0) {
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12; /* next index to compute will be 12 */
    }

    if (n < cache_len)
        return cache[n];

    while (cache_len <= n) {
        /* recurrence for n >= 12 */
        seq_t val = cache[cache_len - 1] * 2;
        val -= cache[cache_len - 2];
        val += cache[cache_len - 8];
        val -= cache[cache_len - 9] * 2;
        val += cache[cache_len - 10];
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
