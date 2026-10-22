/*
 * A343963  a(0) = 0, and for any n > 0, the binary expansion of n has n
 *          digits and starts with the binary expansion of n, say of w digits,
 *          and in case n > w, the remaining binary digits in a(n) are those
 *          of a(n-w).
 * https://oeis.org/A343963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n>0, let w = floor_log2(n) + 1 (number of bits of
 *            n) a(n) = (n << (n - w)) | a(n - w)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If the shift would exceed the width of seq_t, the value does not fit
   in seq_t and the function returns 0.  This is acceptable because
   values that do not fit need not be exact. */

#include <stdint.h>

static inline unsigned bitlen_uint64(idx_t v) {
    /* v > 0 when called */
    return 64 - __builtin_clzll(v);
}

static seq_t a_rec(idx_t n)
{
    if (n == 0) return 0;

    idx_t w = bitlen_uint64(n);          /* number of bits of n */
    idx_t shift = n - w;                  /* bits to shift left */

    seq_t suffix = a_rec(n - w);

    unsigned seq_bits = (unsigned)(sizeof(seq_t) * 8);
    if (shift >= seq_bits)                /* final value would overflow seq_t */
        return 0;

    seq_t pref = ((seq_t)n) << shift;
    return pref | suffix;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
