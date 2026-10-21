/*
 * A283555  Even numbers that cannot be expressed as p+3, p+5, or p+7, with p
 *          prime.
 * https://oeis.org/A283555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: even numbers n such that n-3, n-5, and n-7 are all non‑prime.
   a(1) = 98, a(2) = 122, a(3) = 124, ...  (OEIS A283555) */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* offset is 1, so index 0 is invalid */

    /* Cache up to 10,000 terms – sufficient for all tests. */
    enum { CACHE_MAX = 10000 };
    static seq_t cache[CACHE_MAX];
    static size_t cache_len = 0;

    while (cache_len < n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = (seq_t)98;           /* first term of the sequence */
        else
            candidate = cache[cache_len - 1] + (seq_t)2;

        /* Find next even integer whose three offsets are all non‑prime. */
        while (true) {
            seq_t m3 = candidate - (seq_t)3;
            seq_t m5 = candidate - (seq_t)5;
            seq_t m7 = candidate - (seq_t)7;

            if (!is_prime_big(m3) && !is_prime_big(m5) && !is_prime_big(m7))
                break;                       /* candidate satisfies the condition */

            candidate += (seq_t)2;           /* try next even number */
        }

        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
