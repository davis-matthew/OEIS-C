/*
 * A383270  Length of the longest sequence of contiguous 1s in the binary
 *          expansion of n after flipping at most one 0-bit to 1.
 * https://oeis.org/A383270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the length of the longest contiguous run of 1s in the
 *            binary * expansion of n after flipping at most one 0-bit to 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used by idx_t */

seq_t a(idx_t n)
{
    /* Special case n == 0: binary representation is implicitly "0",
       flipping that bit yields a single 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute bit length of n (number of bits needed to represent n). */
    unsigned bitlen = 0;
    idx_t temp = n;
    while (temp != 0) {
        ++bitlen;
        temp >>= 1;
    }

    /* Arrays for consecutive 1s counts to the left and to the right of each bit. */
    unsigned left[65] = {0};   /* left[i] = number of consecutive 1s ending at position i-1 */
    unsigned right[65] = {0};  /* right[i] = number of consecutive 1s starting at position i */

    /* Fill left array (scan from LSB to MSB). */
    for (unsigned i = 0; i < bitlen; ++i) {
        if ((n >> i) & 1ULL) {
            left[i + 1] = left[i] + 1;
        } else {
            left[i + 1] = 0;
        }
    }

    /* Fill right array (scan from MSB to LSB). */
    right[bitlen] = 0;
    for (int i = (int)bitlen - 1; i >= 0; --i) {
        if ((n >> i) & 1ULL) {
            right[i] = right[i + 1] + 1;
        } else {
            right[i] = 0;
        }
    }

    /* Compute maximum run length, considering both the original runs and
       potential extensions by flipping a single 0-bit. */
    seq_t maxrun = 0;
    for (unsigned i = 0; i < bitlen; ++i) {
        /* Original run length starting at bit i. */
        if (right[i] > maxrun) {
            maxrun = right[i];
        }

        /* If bit i is 0, flipping it could create a longer run. */
        if (!((n >> i) & 1ULL)) {
            seq_t cand = (seq_t)left[i] + 1 + (seq_t)right[i + 1];
            if (cand > maxrun) {
                maxrun = cand;
            }
        }
    }

    return maxrun;
}
