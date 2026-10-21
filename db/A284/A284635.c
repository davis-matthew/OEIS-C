/*
 * A284635  Numbers with digits 6 and 8 only.
 * https://oeis.org/A284635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th smallest positive integer consisting only of the digits 6 and 8. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the length L of the digit string: L = floor(log2(n+1)) */
    idx_t tmp = n + 1;
    unsigned L;
    if (tmp == 0) {            /* overflowed: n == UINT64_MAX -> L == 64 */
        L = 64;
    } else {
        L = 63 - __builtin_clzll(tmp);
    }

    /* Compute the offset within the block of length L: offset = n - (2^L - 1) */
    idx_t offset;
    if (L == 64) {
        offset = 0;
    } else {
        offset = n - (((idx_t)1 << L) - 1);
    }

    /* Build the decimal number by mapping bits of offset to digits 6 and 8. */
    seq_t res = 0;
    for (unsigned i = 0; i < L; ++i) {
        unsigned bit = (offset >> (L - 1 - i)) & 1U;
        res = res * 10 + (bit ? 8 : 6);
    }
    return res;
}
