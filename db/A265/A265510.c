/*
 * A265510  a(n) = largest base-2 palindrome m <= 2n+1 such that every base-2
 *          digit of m is <= the corresponding digit of 2n+1; m is written in
 *          base 2.
 * https://oeis.org/A265510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal representation of the largest base‑2 palindrome m
 *            * such that m ≤ 2n+1 and each binary digit of m is ≤ the
 *            corresponding * binary digit of 2n+1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

/* Reverse the lowest 'len' bits of x. */
static seq_t reverse_bits(seq_t x, unsigned int len)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < len; ++i) {
        r <<= 1;
        r |= (x >> i) & 1u;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* t = 2n + 1 */
    seq_t t = ((seq_t)n << 1) | 1;

    /* length of the binary representation of t */
    unsigned int len = bitlen_seq_t(t);

    /* largest palindrome with digits ≤ those of t */
    seq_t rev = reverse_bits(t, len);
    seq_t m   = t & rev;            /* binary palindrome */

    /* convert the binary string to a decimal number written with digits
       0/1 (e.g. binary 101 → decimal 101). */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        result = result * 10;
        result += (m >> i) & 1u;
    }
    return result;
}
