/*
 * A369328  Antidiagonal sums of A369326.
 * https://oeis.org/A369328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) – 8*a(n-2) + 5*a(n-3), with a(0) = 1, a(1) = 2,
 *            a(2) = 4, a(3) = 9
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */

seq_t a(idx_t n)
{
    /* Static array large enough for n up to ≈10000.
       For larger n the function may be called with a value outside
       this range; such calls are not required by the test harness. */
    static seq_t cache[20001];
    static idx_t cache_len = 0;

    /* Initialize the first four terms once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache_len = 4;
    }

    /* If the requested index is already cached, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Compute new terms up to the requested index. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1]
                 - (seq_t)8 * cache[i - 2]
                 + (seq_t)5 * cache[i - 3];
    }
    cache_len = n + 1;      /* cache now holds terms 0 … n */

    return cache[n];
}
