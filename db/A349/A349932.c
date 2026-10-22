/*
 * A349932  Product of n and its binary ones' complement.
 * https://oeis.org/A349932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n * A035327(n) (binary complement within bit‑length)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            // offset 1, but guard for completeness
        return 0;

    /* Determine the bit‑length of n (position of highest 1 bit + 1) */
    unsigned bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Build a mask with the lowest 'bitlen' bits set to 1 */
    seq_t mask = 0;
    for (unsigned i = 0; i < bitlen; ++i) {
        mask = (mask << 1) | 1;
    }

    /* Complement n within its bit‑length and multiply */
    seq_t comp = mask ^ (seq_t)n;
    return (seq_t)n * comp;
}
