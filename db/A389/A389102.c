/*
 * A389102  a(n) = Sum_{k=0..n} gcd(n^k, k). Antidiagonal sums of A389101.
 * https://oeis.org/A389102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} gcd(n^k, k)
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t sum = 1;                       /* contribution from k = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t r = mod_pow_uint64((uint64_t)n, (uint64_t)k, (uint64_t)k);
        seq_t g = gcd((seq_t)r, (seq_t)k);
        sum += (uint64_t)g;
    }
    return (seq_t)sum;
}
