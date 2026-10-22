/*
 * A331852  a(n) is the number of distinct values obtained by partitioning the
 *          binary representation of n into consecutive blocks, and then
 *          applying the bitwise XOR operator to the numbers represented by
 *          the blocks.
 * https://oeis.org/A331852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct XOR values obtained by partitioning
 *            the binary representation of n into consecutive blocks and
 *            XORing the resulting integers
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT – Brute‑force enumeration of all partitions (≤ 14 bits). */

#include <stdint.h>
#include <string.h>

/* maximum 2^14 bits required for XOR result set */
static unsigned char seen_bits[2048];   /* 2048 bytes = 16384 bits */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* "0" has one partition giving XOR 0 */

    /* Convert n to binary bits, most significant first. */
    uint64_t tmp = (uint64_t)n;
    int bits[64];
    int L = 0;
    while (tmp) {
        bits[L++] = tmp & 1ULL;
        tmp >>= 1U;
    }
    int msb[64];
    for (int i = 0; i < L; ++i)
        msb[i] = bits[L - 1 - i];

    /* Enumerate all 2^(L-1) partitions. */
    size_t max_subsets = (size_t)1 << (L - 1);     /* number of masks */
    memset(seen_bits, 0, sizeof(seen_bits));

    unsigned count = 0;    /* result fits in 32‑bit for n <= 10000 */

    for (size_t mask = 0; mask < max_subsets; ++mask) {
        uint64_t xorv = 0;
        uint64_t cur = 0;
        for (int i = 0; i < L; ++i) {
            cur = (cur << 1) | (uint64_t)msb[i];
            if (i < L - 1 && ((mask >> i) & 1ULL)) {
                xorv ^= cur;
                cur = 0;
            }
        }
        xorv ^= cur;     /* last block */

        size_t idx  = (size_t)xorv;          /* idx ∈ [0, 2^L-1] ≤ 16383 */
        size_t word = idx >> 3;              /* byte index in seen_bits */
        unsigned char bit = 1u << (idx & 7u);
        if (!(seen_bits[word] & bit)) {
            seen_bits[word] |= bit;
            ++count;
        }
    }

    return (seq_t)count;
}
