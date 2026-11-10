/*
 * A339813  The exponent of the highest power of 2 dividing (A019565(n) - 1).
 * https://oeis.org/A339813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A019565(n) - 1), where A019565(n) = Product_{k in
 *            bits of n} prime(k+1)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 6494 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A339813_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    seq_t prod = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1) prod *= (seq_t)A339813_primes[bit];
        n >>= 1;
        ++bit;
    }
    seq_t m = prod - 1;
    seq_t cnt = 0;
    if (m == 0) return 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++cnt;
    }
    return cnt;
}
