/*
 * A328282  a(n) is the least k such that A175930(k) = n.
 * https://oeis.org/A328282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A328282:
 *   a(n) = the least k such that A175930(k) = n
 *   A175930(k) = number of runs of equal bits in the binary representation of k
 *
 * Formula: a(n) is constructed from the binary representation of n.
 *   Let n have k ones.  The binary representation of a(n) has k runs of
 *   equal bits.  The i‑th run (starting with 1)s length is 2^z, where
 *   z is the number of consecutive zeros immediately following the i‑th
 *   one bit in n.  The runs alternate between 1 and 0, starting with 1.
 *
 * This implementation follows that construction directly.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 is guaranteed by the caller. */
    if (n == 0) {
        return 0;        /* not defined for n = 0, but defensive */
    }

    /* Determine the positions of all 1‑bits in n, stored from MSB to LSB. */
    idx_t pos[64];          /* idx_t is uint64_t or similar, so 64 bits max */
    size_t ones = 0;
    size_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }
    /* Extract positions */
    for (size_t i = 0; i < bitlen; ++i) {
        idx_t bitpos = bitlen - 1 - i;            /* MSB to LSB */
        if ((n >> bitpos) & (idx_t)1) {
            pos[ones++] = bitpos;
        }
    }

    /* Build the binary representation of a(n). */
    seq_t result = 0;
    int bit_val = 1;                     /* start with a run of 1s */
    for (size_t idx = 0; idx < ones; ++idx) {
        /* Compute z: number of consecutive zeros after this 1. */
        idx_t z;
        if (idx + 1 < ones) {
            z = pos[idx] - pos[idx + 1] - 1;
        } else {
            z = pos[idx];                /* trailing zeros after the last 1 */
        }

        /* Run length = 2^z. For z <= 63 the value fits in size_t. */
        size_t runlen = 1ULL << z;

        /* Append runlen bits with current bit value. */
        for (size_t j = 0; j < runlen; ++j) {
            result = (result << 1) | (seq_t)bit_val;
        }

        bit_val ^= 1;                 /* toggle bit for next run */
    }

    return result;
}
