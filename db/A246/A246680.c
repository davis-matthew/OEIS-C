/*
 * A246680  Permutation of natural numbers, even bisection of A246676 halved:
 *          a(n) = A246676(2*n)/2.
 * https://oeis.org/A246680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246676(2*n)/2, where A246676(m) = A242378(A007814(m),
 *            1+A000265(m)) - 1: write m = 2^k * odd, factor odd+1, replace
 *            each prime by the prime k places later, multiply, subtract 1
 * Range:     u8 n <= 27, u16 n <= 247, u32 n <= 8175, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 90 terms through n = 7839 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A246680_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A246680_nextp(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A246680_isp(q)) ++q;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t m2 = (uint64_t)n * 2;
    unsigned k = 0;
    while ((m2 & 1) == 0) { m2 >>= 1; ++k; }
    uint64_t mm = m2 + 1;   /* odd part + 1 */

    seq_t product = (seq_t)1;
    for (uint64_t p = 2; p * p <= mm; ++p) {
        if (mm % p == 0) {
            int e = 0;
            while (mm % p == 0) { mm /= p; ++e; }
            uint64_t q = p;
            for (unsigned i = 0; i < k; ++i) q = A246680_nextp(q);
            while (e--) product *= (seq_t)q;
        }
    }
    if (mm > 1) {
        uint64_t q = mm;
        for (unsigned i = 0; i < k; ++i) q = A246680_nextp(q);
        product *= (seq_t)q;
    }
    return (product - (seq_t)1) / 2;
}
