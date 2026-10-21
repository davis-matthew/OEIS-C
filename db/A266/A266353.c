/*
 * A266353  Expansion of b(3)*b(4)/(1 - 2*x + x^2 - x^3 + x^4), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑3) + 12 for n >= 5 Initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=20, a(4)=35
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 51, u64 n <= 109, u128 n <= 226,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache up to a reasonable maximum (n up to ~10000). */
    static seq_t cache[20001];
    static size_t cached_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Set known initial terms. */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 10;
        cache[3] = 20;
        cache[4] = 35;
        cached_len = 5;          /* next index to compute */
        initialized = true;
    }

    size_t idx = (size_t)n;

    if (idx < cached_len) {
        return cache[idx];
    }

    /* Compute up to the requested index. */
    for (size_t i = cached_len; i <= idx; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + (seq_t)12;
    }
    cached_len = idx + 1;
    return cache[idx];
}
