/*
 * A379501  a(n) = (3/2)*A019565((2n-1)^2) - A019565(A001065((2n-1)^2)), where
 *          A019565 is the base-2 exp-function, and A001065 is the sum of
 *          proper divisors of n.
 * https://oeis.org/A379501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (3/2)*A019565((2n-1)^2) - A019565(A001065((2n-1)^2))
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 37, i64 n <= 313,
 *            i128 n <= 15797, big any n
 * Validated: standard, pass, 54 terms through n = 9036 (2026-11-10)
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

static seq_t A019565_a(uint64_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0 && bit < 64) {
        if (n & 1ULL) {
            result *= (seq_t)A019565_primes[bit];
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

static uint64_t A001065_a(uint64_t n)
{
    uint64_t sum = 0;
    for (uint64_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) sum += i;
            uint64_t c = n / i;
            if (c < n && c != i) sum += c;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t m = (2 * (uint64_t)n - 1) * (2 * (uint64_t)n - 1);
    seq_t x = A019565_a(m);          /* even, since m is odd */
    seq_t t = (x / 2) * 3;
    return t - A019565_a(A001065_a(m));
}
