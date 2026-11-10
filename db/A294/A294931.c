/*
 * A294931  Multiplicative with a(p^e) = A019565(A289813(e)).
 * https://oeis.org/A294931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} A019565(A289813(e)); A289813(0)=0,
 *            A289813(3m)=2*A289813(m), A289813(3m+1)=2*A289813(m)+1,
 *            A289813(3m+2)=2*A289813(m)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 135 terms through n = 9068 (2026-11-08)
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

static uint64_t A289813_recur(uint64_t x)
{
    if (x == 0) return 0;
    uint64_t r = A289813_recur(x / 3);
    if (x % 3 == 1) return (r << 1) | 1;
    return r << 1;
}

static seq_t A019565_a(uint64_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1ULL) result *= (seq_t)A019565_primes[bit];
        n >>= 1;
        ++bit;
    }
    return result;
}

static seq_t exp_value(uint64_t e)
{
    return A019565_a(A289813_recur(e));
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= exp_value(e);
        }
    }
    if (m > 1) result *= exp_value(1);
    return result;
}
