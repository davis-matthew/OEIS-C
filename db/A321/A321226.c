/*
 * A321226  Describe the binary representation of n in binary and convert back
 *          to decimal.
 * https://oeis.org/A321226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary run‑length encoding of n’s binary representation
 *            where each run length is written in binary followed by the bit
 *            of the run, concatenated into a binary string, then converted to
 *            decimal. Example: n = 67 (binary 1000011) → runs 1(1)·4(0)·2(1)
 *            → binary 111000101 → 453
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t bitlen_uint64(idx_t v) {
    return v == 0 ? 1 : 64 - __builtin_clzll(v);
}

seq_t a(idx_t n) {
    /* Handle binary length of n (0 has length 1). */
    idx_t numbits = (n == 0) ? 1 : bitlen_uint64(n);

    seq_t result = 0;
    idx_t runlen = 0;
    uint8_t current_bit = 0;

    /* Process bits from most significant to least significant. */
    for (idx_t pos = 0; pos < numbits; ++pos) {
        uint8_t bit = (n >> (numbits - 1 - pos)) & 1ULL;

        if (pos == 0) {
            current_bit = bit;
            runlen = 1;
            continue;
        }

        if (bit == current_bit) {
            ++runlen;
        } else {
            /* Flush the run. */
            idx_t len_bits = bitlen_uint64(runlen);
            result = (result << (len_bits + 1)) |
                     (((seq_t)runlen << 1) | current_bit);

            current_bit = bit;
            runlen = 1;
        }
    }

    /* Flush the final run. */
    if (numbits > 0) {
        idx_t len_bits = bitlen_uint64(runlen);
        result = (result << (len_bits + 1)) |
                 (((seq_t)runlen << 1) | current_bit);
    }

    return result;
}
