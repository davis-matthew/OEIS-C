/*
 * A392875  a(n) = A019565(n) mod A117366(A276086(n)), where A117366 gives the
 *          smallest prime greater than the largest prime dividing n, and
 *          A019565 is the base-2 exp-function.
 * https://oeis.org/A392875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A019565(n) mod prime(L+1), where L = bit length of n (L =
 *            0 for n = 0), and A019565(n) = Product_{k in bits of n}
 *            prime(k+1)
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 109 terms through n = 9758 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A392875_primes[66] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311,
    313, 317
};

seq_t a(idx_t n)
{
    unsigned len = 0;
    idx_t t = n;
    while (t) { ++len; t >>= 1; }
    uint64_t p = A392875_primes[len];   /* prime(len+1) */
    uint64_t r = 1 % p;
    unsigned bit = 0;
    t = n;
    while (t) {
        if (t & 1ULL)
            r = (r * (uint64_t)A392875_primes[bit]) % p;
        t >>= 1;
        ++bit;
    }
    return (seq_t)r;
}
