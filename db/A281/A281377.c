/*
 * A281377  Write n in Elias's omega code, interchange the 1's and 0's and
 *          convert it back to decimal.
 * https://oeis.org/A281377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value obtained by flipping the bits of the * Elias omega
 *            code of n (as defined in OEIS A281377)
 * Range:     u8 n <= 15, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t, etc. */

static inline unsigned bitlen_idx(idx_t x)
{
    /* bit length of a positive idx_t value */
    return 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0)  /* below offset – not used, but safe */
        return (seq_t)0;

    /* Build the chain of n values used in the omega encoding. */
    idx_t stack[64];
    int top = 0;
    idx_t tmp = n;

    while (tmp > 1) {
        stack[top++] = tmp;
        tmp = bitlen_idx(tmp) - 1;
    }

    /* Accumulate the flipped bits into the result. */
    seq_t result = (seq_t)0;

    for (int i = top - 1; i >= 0; --i) {
        idx_t v = stack[i];
        unsigned len = bitlen_idx(v);
        for (int j = len - 1; j >= 0; --j) {
            unsigned bit = (v >> j) & 1U;
            unsigned flipped = bit ? 0U : 1U; /* 1 - bit */
            result = (result << 1) | (seq_t)flipped;
        }
    }

    /* Add the flipped terminator bit (flip of 0 is 1). */
    result = (result << 1) | (seq_t)1;

    return result;
}
