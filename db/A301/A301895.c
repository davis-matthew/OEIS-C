/*
 * A301895  a(n) = (number of 1's in binary expansion of n)^(number of 0's in
 *          binary expansion of n).
 * https://oeis.org/A301895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (number of 1's in binary expansion of n)^(number of 0's
 *            in binary expansion of n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the integer a(n) for the OEIS sequence A301895. */
seq_t a(idx_t n)
{
    /* Count ones in n. For idx_t (uint64_t) the builtin works. */
    unsigned ones = __builtin_popcountll(n);

    /* Determine the number of bits in the binary expansion of n.
       For n==0 we treat it as having one bit ("0"). */
    unsigned bitlen;
    if (n == 0) {
        bitlen = 1;
    } else {
        /* Count bits by shifting. */
        bitlen = 0;
        idx_t tmp = n;
        while (tmp) {
            bitlen++;
            tmp >>= 1;
        }
    }

    /* Number of zeros = total bits - ones. */
    unsigned zeros = bitlen - ones;

    /* Compute base^exponent using simple multiplication. */
    seq_t res = 1;
    seq_t base = (seq_t)ones;
    for (unsigned i = 0; i < zeros; ++i) {
        res *= base;
    }
    return res;
}
