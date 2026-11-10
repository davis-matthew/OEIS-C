/*
 * A366275  The Cat's tongue permutation: a(n) = A163511(A057889(n)).
 * https://oeis.org/A366275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A163511(A057889(n)); A163511(0) = 1, A163511(m) = p(r) *
 *            Product_{i=1..r} p(i)^(b_i - b_{i-1} - 1), where b_1 < ... < b_r
 *            are the 1-based positions of the 1-bits of m, b_0 = 0, p(i) is
 *            the i-th prime, and A057889 reverses the bits of the odd part of
 *            n while keeping its trailing zeros
 * Range:     u8 n <= 40, u16 n <= 384, u32 n <= 11520, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 90 terms through n = 9274 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A366275_rev(idx_t n)
{
    if (n == 0) return 0;
    unsigned tz = 0;
    while (((n >> tz) & 1ULL) == 0ULL) ++tz;
    idx_t m = n >> tz;
    idx_t rev = 0;
    while (m) {
        rev = (rev << 1) | (m & 1ULL);
        m >>= 1;
    }
    return rev << tz;
}

static uint64_t A366275_prime(unsigned k)
{
    /* k-th prime, k >= 1, by trial division */
    unsigned cnt = 0;
    uint64_t c = 1;
    while (cnt < k) {
        ++c;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) ++cnt;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t m = A366275_rev(n);
    if (m == 0) return (seq_t)1;

    unsigned pos[64];
    unsigned cnt = 0;
    unsigned bitpos = 1;
    idx_t t = m;
    while (t) {
        if (t & 1ULL) pos[cnt++] = bitpos;
        ++bitpos;
        t >>= 1;
    }

    seq_t result = 1;
    unsigned prev = 0;
    uint64_t p = 0;
    for (unsigned i = 0; i < cnt; ++i) {
        p = A366275_prime(i + 1);
        unsigned e = pos[i] - prev - 1;
        for (unsigned j = 0; j < e; ++j) result *= (seq_t)p;
        prev = pos[i];
    }
    result *= (seq_t)p; /* p(r), r = popcount(m) */
    return result;
}
