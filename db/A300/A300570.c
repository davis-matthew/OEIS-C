/*
 * A300570  a(n) is the concatenation n in base 2, n-1 in base 2, ..., 1 in
 *          base 2.
 * https://oeis.org/A300570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number obtained by concatenating the * binary
 *            representations of n, n-1, … , 1, each binary string interpreted
 *            * as decimal digits
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* pow10[i] = 10^i for i up to the maximum bit length needed.
       The table is built lazily and reused for subsequent calls. */
    static seq_t pow10[65];
    static unsigned pow10_max = 0;

    if (n == 0) return 0;                 /* undefined, but safe */

    unsigned maxbits = (unsigned)(floor_log2_uint64(n) + 1);

    if (maxbits > pow10_max) {
        if (pow10_max == 0) {
            pow10[0] = 1;
            pow10_max = 1;
        }
        for (unsigned i = pow10_max; i <= maxbits; ++i) {
            pow10[i] = pow10[i - 1] * 10;
        }
        pow10_max = maxbits;
    }

    seq_t res = 0;

    for (idx_t k = n; k >= 1; --k) {
        unsigned bits = (unsigned)(floor_log2_uint64(k) + 1);

        /* Convert the binary representation of k into a decimal
           number consisting of its bits as digits. */
        seq_t bin_val = 0;
        for (idx_t i = bits; i > 0; --i) {
            unsigned bit = (unsigned)((k >> (i - 1)) & 1u);
            bin_val = bin_val * 10 + bit;
        }

        /* Append the decimal representation of the bits to res. */
        res = res * pow10[bits] + bin_val;
    }

    return res;
}
