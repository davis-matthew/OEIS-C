/*
 * A345391  a(n) is the least proper multiple of n with the same set of
 *          decimal digits as n.
 * https://oeis.org/A345391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least proper multiple of n with the same set of decimal
 *            digits as n
 * Range:     u8 n <= 10, u16 n <= 110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_digits.h"

/* Return a bitmask (bit i set iff digit i appears in x). */
static inline uint16_t digit_set_mask(seq_t x)
{
    uint8_t buf[128];
    size_t len;
    get_digits(x, buf, &len);
    uint16_t mask = 0;
    for (size_t i = 0; i < len; ++i)
        mask |= 1U << buf[i];
    return mask;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;
    uint16_t mask_n = digit_set_mask(n_seq);

    /* Search for the smallest proper multiple whose digit set matches. */
    for (idx_t k = 2;; ++k)
    {
        seq_t m = n_seq * (seq_t)k;
        if (digit_set_mask(m) == mask_n)
            return m;
    }
}
