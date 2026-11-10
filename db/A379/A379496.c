/*
 * A379496  a(n) = A019565(1+n) - A019565(A001065(n)), where A019565 is the
 *          base-2 exp-function, and A001065 is the sum of proper divisors of
 *          n.
 * https://oeis.org/A379496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A019565(1+n) - A019565(A001065(n)), A019565(m) =
 *            Product_{k in bits of m} prime(k+1), A001065(n) = sigma(n) - n
 * Range:     i8 n <= 13, i16 n <= 71, i32 n <= 839, i64 n <= 32768,
 *            i128 n <= 32768, big any n
 * Validated: standard, pass, 79 terms through n = 9669 (2026-11-10)
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

static seq_t A019565_a(idx_t n)
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

static idx_t A001065_a(idx_t n)
{
    idx_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) sum += i;
            idx_t c = n / i;
            if (c < n && c != i) sum += c;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    return A019565_a(n + 1) - A019565_a(A001065_a(n));
}
