/*
 * A296421  a(1) = 1, a(2) = 2, a(3) = 3, a(4) = a(5) = 4, a(6) = a(7) = 6;
 *          a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 7.
 * https://oeis.org/A296421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)); Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=a(5)=4, a(6)=a(7)=6
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0;           /* offset starts at 1 */

    if (n <= cache_len) {
        return cache[n - 1];
    }

    size_t new_len = (size_t)n;
    seq_t *tmp;
    if (cache == NULL) {
        tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (!tmp) abort();
    cache = tmp;
    size_t prev_len = cache_len;
    cache_len = new_len;

    for (idx_t i = (idx_t)(prev_len + 1); i <= n; ++i) {
        seq_t val;
        if (i == 1)                     val = (seq_t)1;
        else if (i == 2)                val = (seq_t)2;
        else if (i == 3)                val = (seq_t)3;
        else if (i == 4 || i == 5)      val = (seq_t)4;
        else if (i == 6 || i == 7)      val = (seq_t)6;
        else {
            idx_t idx1 = i - (idx_t)cache[i - 2];
            idx_t idx2 = i - (idx_t)cache[i - 3];
            idx_t idx3 = i - (idx_t)cache[i - 4];
            val = cache[idx1 - 1] + cache[idx2 - 1] + cache[idx3 - 1];
        }
        cache[i - 1] = val;
    }

    return cache[n - 1];
}
