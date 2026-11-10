/*
 * A300830  a(n) = Product_{d|n} A019565(d)^(1-A008966(n/d)).
 * https://oeis.org/A300830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A019565(d)^(1-A008966(n/d)), where
 *            A008966(m)=1 if m squarefree else 0
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 719, big any n
 * Validated: standard, pass, 106 terms through n = 8045 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A300830_primes[64] = {
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
    while (n != 0) {
        if (n & 1ULL) result *= (seq_t)A300830_primes[bit];
        n >>= 1;
        ++bit;
    }
    return result;
}

static int A008966_a(idx_t n)
{
    if (n < 2) return 1;
    idx_t t = n;
    for (idx_t i = 2; i <= t / i; ++i) {
        if (t % i == 0) {
            int e = 0;
            do { t /= i; ++e; } while (t % i == 0);
            if (e > 1) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;
        idx_t j = n / i;
        /* d = i, cofactor j */
        if (!A008966_a(j)) result *= A019565_a(i);
        if (j != i) {
            /* d = j, cofactor i */
            if (!A008966_a(i)) result *= A019565_a(j);
        }
    }
    return result;
}
