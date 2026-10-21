/*
 * A245270  Like A067599 but write everything in binary, then display the
 *          answer in base 10.
 * https://oeis.org/A245270
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary encoding of the prime‑factorization of n, where
 *            each factor p^e contributes the bits of p followed by the bits
 *            of e (no leading zeros); the concatenated bit string is
 *            interpreted as a binary integer and returned in decimal
 * Range:     u8 n <= 29, u16 n <= 1019, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* undefined for n < 2, but safe */

    seq_t result = 0;
    idx_t temp   = n;

    /* Handle factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        unsigned int bp = bitlen_seq_t((seq_t)2);
        unsigned int be = bitlen_seq_t((seq_t)exp);
        seq_t block = ((seq_t)2 << be) | (seq_t)exp;
        result = (result << (bp + be)) | block;
    }

    /* Handle odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            unsigned int bp = bitlen_seq_t((seq_t)p);
            unsigned int be = bitlen_seq_t((seq_t)exp);
            seq_t block = ((seq_t)p << be) | (seq_t)exp;
            result = (result << (bp + be)) | block;
        }
    }

    /* If remaining temp is a prime > sqrt(original n) */
    if (temp > 1) {
        idx_t exp = 1;
        unsigned int bp = bitlen_seq_t((seq_t)temp);
        unsigned int be = bitlen_seq_t((seq_t)exp);
        seq_t block = ((seq_t)temp << be) | (seq_t)exp;
        result = (result << (bp + be)) | block;
    }

    return result;
}
