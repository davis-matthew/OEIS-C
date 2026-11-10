/*
 * A351558  a(n) = A048675(gcd(n, A019565(n))).
 * https://oeis.org/A351558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A048675(gcd(n, A019565(n))) = sum of 2^k over set bits k
 *            of n with prime(k+1) dividing n (equivalently n AND A087207(n))
 * Range:     u8 n <= 275, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 9716 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A351558_primes[64] = {
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
    seq_t result = 0;
    seq_t p2 = 1;
    idx_t m = n;
    unsigned k = 0;
    while (m != 0 && k < 64) {
        if (m & 1) {
            if (n % (idx_t)A351558_primes[k] == 0) {
                result += p2;
            }
        }
        m >>= 1;
        p2 = p2 + p2;
        ++k;
    }
    return result;
}
