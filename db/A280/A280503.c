/*
 * A280503  a(n) = A091255(n,A056539(n)).
 * https://oeis.org/A280503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = gcd_GF2[x](n, A056539(n)), the polynomial gcd over GF(2)
 *            of the binary expansions of n and A056539(n) (A091255(n,k) is
 *            this gcd)
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 128 terms through n = 7563 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A280503_deg(uint64_t p)
{
    return 63 - __builtin_clzll(p);
}

static uint64_t A280503_pmod(uint64_t a, uint64_t b)
{
    int db = A280503_deg(b);
    while (a != 0 && A280503_deg(a) >= db)
        a ^= b << (A280503_deg(a) - db);
    return a;
}

static uint64_t A280503_pgcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t r = A280503_pmod(a, b);
        a = b;
        b = r;
    }
    return a;
}

static uint64_t A280503_perm(uint64_t m)
{
    if (m == 0)
        return 0;
    int bitlen = 64 - __builtin_clzll(m);
    int runs = 1;
    int prev = (int)((m >> (bitlen - 1)) & 1);
    for (int i = bitlen - 2; i >= 0; --i) {
        int bit = (int)((m >> i) & 1);
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
    }
    uint64_t rev = 0;
    for (int i = 0; i < bitlen; ++i)
        rev = (rev << 1) | ((m >> i) & 1);
    if ((runs & 1) == 0) {
        uint64_t mask = (bitlen == 64) ? ~(uint64_t)0 : (((uint64_t)1 << bitlen) - 1);
        rev ^= mask;
    }
    return rev;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    return (seq_t)A280503_pgcd(m, A280503_perm(m));
}
