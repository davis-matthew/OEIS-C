/*
 * A379495  a(n) = A019565(A001065(n)), where A019565 is the base-2
 *          exp-function, and A001065 is the sum of proper divisors of n.
 * https://oeis.org/A379495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A019565(A001065(n)); A001065(n) = sigma(n) - n;
 *            A019565(m) = Product_{k in binary set bits of m} prime(k+1)
 * Range:     u8 n <= 27, u16 n <= 71, u32 n <= 839, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 93 terms through n = 9158 (2026-11-10)
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

static seq_t A019565_a(seq_t m)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (m != 0 && bit < 64) {
        if ((m & 1) != 0) {
            result *= (seq_t)A019565_primes[bit];
        }
        m >>= 1;
        ++bit;
    }
    return result;
}

static seq_t A001065_a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) {
                sum += (seq_t)i;
            }
            idx_t complement = n / i;
            if (complement < n && complement != i) {
                sum += (seq_t)complement;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    return A019565_a(A001065_a(n));
}
