/*
 * A280508  a(n) = n XOR A057889(n).
 * https://oeis.org/A280508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n XOR A057889(n)
 * Range:     u8 n <= 514, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057889(n) is the binary reflection of n padded with t trailing zeros, where t = number of trailing zeros in n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert n to 64‑bit for sub‑calculations */
    uint64_t x = (uint64_t)n;

    /* Count number of trailing zeros and bit length of n */
    int tz = __builtin_ctzll(x);
    int bl = 64 - __builtin_clzll(x);
    int width = bl + tz;          /* total number of bits to be reversed */

    /* Reverse the bits of x in 'width' bits */
    unsigned __int128 rev = 0;
    for (int i = 0; i < width; ++i) {
        if ((x >> i) & 1ULL) {
            rev |= ((unsigned __int128)1 << (width - 1 - i));
        }
    }

    seq_t rev_seq = (seq_t)rev;   /* truncate or widen as needed */
    seq_t n_seq   = (seq_t)n;
    return n_seq ^ rev_seq;       /* a(n) = n XOR A057889(n) */
}
