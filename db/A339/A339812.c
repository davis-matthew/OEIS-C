/*
 * A339812  Number of prime divisors of (A019565(n) - 1), counted with
 *          multiplicity.
 * https://oeis.org/A339812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(A019565(n) - 1), where A019565(n) = Product_{bit
 *            k set in n} prime(k+1)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9926 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static const uint32_t A339812_primes[64] = {
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
    if (n < 32768ULL) {
        /* product of at most the first 15 primes fits in uint64_t */
        uint64_t prod = 1;
        idx_t m = n;
        int bit = 0;
        while (m) {
            if (m & 1ULL) prod *= A339812_primes[bit];
            m >>= 1;
            ++bit;
        }
        prod -= 1;
        if (prod <= 1) return (seq_t)0;
        uint64_t pr[24], ex[24];
        size_t cnt = 0;
        factor_u64(prod, pr, ex, &cnt);
        uint64_t total = 0;
        for (size_t i = 0; i < cnt; i++) total += ex[i];
        return (seq_t)total;
    } else {
        seq_t prod = 1;
        idx_t m = n;
        int bit = 0;
        while (m && bit < 64) {
            if (m & 1ULL) prod *= (seq_t)A339812_primes[bit];
            m >>= 1;
            ++bit;
        }
        prod -= 1;
        seq_t count = 0;
        while (prod % 2 == 0 && prod > 1) { count++; prod /= 2; }
        for (seq_t p = 3; p <= prod / p; p += 2) {
            while (prod % p == 0) { count++; prod /= p; }
        }
        if (prod > 1) count++;
        return count;
    }
}
