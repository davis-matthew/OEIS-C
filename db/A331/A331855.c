/*
 * A331855  a(n) is the number of distinct values obtained by partitioning the
 *          binary representation of n into consecutive blocks, and then
 *          reversing those blocks.
 * https://oeis.org/A331855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct values obtained by reversing every
 *            block of a partition of the binary representation of n
 * Range:     u8 n <= 1112, u16 n <= 9565, u32 n <= 9565, u64 n <= 9565,
 *            u128 n <= 9565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* number of bits in n (with n==0 yielding 1 bit "0") */
    unsigned L = 0;
    if (n == 0) {
        L = 1;
    } else {
        idx_t tmp = n;
        while (tmp) {
            L++;
            tmp >>= 1;
        }
    }

    /* store bits of n, most‑significant first */
    unsigned bits[64];
    for (unsigned i = 0; i < L; i++) {
        bits[i] = (unsigned)((n >> (L - 1 - i)) & 1);
    }

    /* maximum value obtainable is (1<<L)-1 */
    unsigned max_val = ((unsigned)1u << L) - 1u;
    bool *seen = (bool *)calloc((size_t)max_val + 1u, sizeof(bool));
    if (seen == NULL) {
        return (seq_t)0;          /* fallback – should not happen */
    }

    unsigned total_masks = (unsigned)1u << (L - 1);   /* L>=1 always */
    unsigned long long count = 0;

    for (unsigned mask = 0; mask < total_masks; mask++) {
        unsigned long long res = 0;
        unsigned start = 0;

        for (unsigned i = 0; i < L - 1; i++) {
            unsigned end = i + 1;
            if (mask & (1u << i)) {
                unsigned long long rev = 0;
                for (int j = (int)end - 1; j >= (int)start; j--) {
                    rev = (rev << 1) | bits[j];
                }
                unsigned len = end - start;
                res = (res << len) | rev;
                start = end;
            }
        }
        /* last block */
        unsigned long long rev = 0;
        for (int j = (int)L - 1; j >= (int)start; j--) {
            rev = (rev << 1) | bits[j];
        }
        unsigned len = L - start;
        res = (res << len) | rev;

        if (!seen[(size_t)res]) {
            seen[(size_t)res] = true;
            count++;
        }
    }

    free(seen);
    return (seq_t)count;
}
