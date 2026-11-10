/*
 * A277811  Column 1 of A277810: a(n) = A019565(A065621(n)).
 * https://oeis.org/A277811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A019565(A065621(n)), A065621(n) = n XOR 2*(n -
 *            lowbit(n)), A019565(m) = Product_{k in bits of m} prime(k+1)
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 597, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 80 terms through n = 7639 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A019565_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static idx_t A065621_idx(idx_t n)
{
    idx_t low = n & (idx_t)(0 - n);
    return n ^ ((n - low) << 1);
}

seq_t a(idx_t n)
{
    idx_t m = A065621_idx(n);
    seq_t result = 1;
    unsigned bit = 0;
    while (m != 0 && bit < 64) {
        if (m & 1) {
            result *= (seq_t)A019565_primes[bit];
        }
        m >>= 1;
        ++bit;
    }
    return result;
}
