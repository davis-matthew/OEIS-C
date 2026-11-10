/*
 * A351557  a(n) = gcd(sigma(n), A019565(n)).
 * https://oeis.org/A351557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), A019565(n)); A019565(n) = Product_{k in
 *            bits of n} prime(k+1)
 * Range:     u8 n <= 22, u16 n <= 222, u32 n <= 8035, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9940 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

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

static seq_t A000203_a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t s = A000203_a(n);
    seq_t q = A019565_a(n);
    return gcd(s, q);
}
