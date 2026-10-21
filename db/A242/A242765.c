/*
 * A242765  a(1) = 2; for n>1, a(n) = product of digits of (a(n-1)^2).
 * https://oeis.org/A242765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2; for n > 1, a(n) = product of digits of (a(n-1)^2)
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

/* Returns the n-th term of A242765 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache the first few terms. The sequence becomes zero from the
       7th term onwards. */
    static seq_t cache[8];
    static bool initialized = false;

    if (!initialized) {
        cache[1] = (seq_t)2;               /* a(1) */
        for (idx_t i = 2; i <= 7; ++i) {
            seq_t sq = cache[i - 1] * cache[i - 1];
            cache[i] = digit_product(sq);
        }
        initialized = true;
    }

    if (n <= 7)
        return cache[n];
    return (seq_t)0;  /* Once we hit 0, all subsequent terms stay 0. */
}
