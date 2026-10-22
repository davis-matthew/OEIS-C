/*
 * A344220  a(n) is the least k >= 0 such that n XOR k is a binary palindrome
 *          (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A344220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that (n ^ k) is a binary palindrome
 * Range:     u8 n <= 436, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>            /* for bool */

/* Return the number of bits needed to represent x (at least 1). */
static unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned len = 0;
    while (x > (seq_t)0) {
        x >>= 1;
        len++;
    }
    return len ? len : 1;
}

/* Linear test for a binary palindrome. */
static bool is_bin_pal_local(seq_t x)
{
    unsigned len = bitlen_seq_t_local(x);
    for (unsigned i = 0; i < len / 2; ++i) {
        unsigned bit_lo = (unsigned)((x >> i) & 1);
        unsigned bit_hi = (unsigned)((x >> (len - 1 - i)) & 1);
        if (bit_lo != bit_hi)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;
    seq_t k   = (seq_t)0;

    for (;;) {
        seq_t x = num ^ k;
        if (is_bin_pal_local(x))
            return k;
        ++k;
    }
}
