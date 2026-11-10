/*
 * A368901  The least common multiple of the first n terms of Doudna sequence,
 *          A005940.
 * https://oeis.org/A368901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = A368900(n)*a(n-1) for n > 0, where A368900(m) =
 *            prime(j+1) if m-1 = 2^i - 2^j (i > j >= 0), else 1. So a(n) =
 *            product of prime(j+1) over all pairs i > j with 2^i - 2^j <= n-1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 48, u64 n <= 128, u128 n <= 960,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 8788 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t A368901_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n < 2)
        return result;
    uint64_t lim = (uint64_t)(n - 1);
    for (unsigned j = 0; j < 64; j++) {
        uint64_t pj = (uint64_t)1 << j;
        if (pj > lim)
            break;
        for (unsigned i = j + 1; i < 64; i++) {
            uint64_t v = ((uint64_t)1 << i) - pj;
            if (v > lim)
                break;
            result *= (seq_t)A368901_primes[j];
        }
    }
    return result;
}
