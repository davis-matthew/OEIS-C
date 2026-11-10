/*
 * A351091  a(n) = Product_{d|A000265(n)} A019565(A289813(d)); a product
 *          obtained from the 1-digits present in ternary expansions of the
 *          odd divisors of n.
 * https://oeis.org/A351091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A351081(A000265(n)) = Product_{d | odd part of n}
 *            Product_{k: ternary digit k of d is 1} prime(k+1)
 * Range:     u8 n <= 38, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint32_t A351091_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A351091_g(idx_t x)
{
    seq_t g = 1;
    unsigned pos = 0;
    while (x > 0) {
        if (x % 3 == 1)
            g *= (seq_t)A351091_primes[pos];
        x /= 3;
        ++pos;
    }
    return g;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while ((n & 1) == 0)
        n >>= 1;

    seq_t result = 1;
    for (idx_t i = 1; i * i <= n; i += 2) {
        if (n % i == 0) {
            result *= A351091_g(i);
            idx_t j = n / i;
            if (j != i)
                result *= A351091_g(j);
        }
    }
    return result;
}
