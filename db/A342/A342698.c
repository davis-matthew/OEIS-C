/*
 * A342698  For any number n with binary expansion (b(1), b(2), ..., b(k)),
 *          the binary expansion of a(n) is (floor((b(k)+b(1)+b(2))/2),
 *          floor((b(1)+b(2)+b(3))/2), ..., floor((b(k-1)+b(k)+b(1))/2)).
 * https://oeis.org/A342698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A342698:
 * a(n) is obtained by taking the binary expansion of n (with a single 0
 * for n = 0), and for each bit output the majority (at least two 1's)
 * of that bit together with its two cyclic neighbours.
 * The result is the integer represented by that new binary string.
 */
#include <stdint.h>

/* Helper to compute the number of bits of an idx_t.
 * For n = 0 the length is defined to be 1. */
static int bitlen_idx(idx_t n)
{
    int len = 0;
    while (n) {
        len++;
        n >>= 1;
    }
    return len ? len : 1;
}

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t sn = (seq_t)n;

    /* Determine the bit length of the binary expansion. */
    int k = bitlen_idx(n);

    /* Build a bit mask for the lower k bits. */
    seq_t mask;
    if (k == 64) {
        mask = ~((seq_t)0);
    } else {
        mask = (((seq_t)1) << k) - 1;
    }

    /* Rotate the bit pattern left and right within the k-bit window. */
    seq_t rotL = (sn << 1) | (sn >> (k - 1));
    rotL &= mask;
    seq_t rotR = (sn >> 1) | (sn << (k - 1));
    rotR &= mask;

    /* Majority of each triple: (a & b) | (a & c) | (b & c) */
    seq_t res = (sn & rotL) | (sn & rotR) | (rotL & rotR);
    res &= mask;  /* Ensure we stay within k bits */

    return res;
}
