/*
 * A380009  Numbers t whose binary expansion Sum 2^e_i has exponents e_i which
 *          are evil numbers (A001969).
 * https://oeis.org/A380009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{i≥0} ( (n >> i) & 1 ) * 2^{evil[i]}
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_evil_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where evil[i] is the ith nonnegative integer with an even number
   of 1-bits in its binary expansion. */

#include <stdint.h>
#include "../utils/is_evil_uint64.h"

seq_t a(idx_t n) {
    /* Compute a(n) for the A380009 sequence. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    /* Number of bits required to represent n. */
    idx_t bits = 64 - __builtin_clzll(n);
    idx_t i;          /* Bit position in n. */
    uint64_t e = 0;   /* Current exponent candidate. */

    for (i = 0; i < bits; ++i) {
        /* Find the i-th evil number. */
        while (!is_evil_uint64(e))
            ++e;

        /* If the i-th bit of n is set, set bit e in result. */
        if (n & (1ull << i))
            result |= ((seq_t)1 << e);

        ++e; /* Prepare for next iteration. */
    }

    return result;
}
