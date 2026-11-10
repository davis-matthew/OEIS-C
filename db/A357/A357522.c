/*
 * A357522  Reverse run lengths in binary expansions of terms of A063037: for
 *          n >= 0, a(n) is the unique k such that A063037(1+k) =
 *          A056539(A063037(1+n)).
 * https://oeis.org/A357522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = k where A063037(1+k) = A056539(A063037(1+n)); computed by
 *            linear scan over numbers without 3 equal consecutive bits
 * Range:     u8 n <= 231, u16 n <= 10944, u32 n <= 10944, u64 n <= 10944,
 *            u128 n <= 10944, big any n
 * Validated: standard, pass, 84 terms through n = 9471 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A357522_ok(uint64_t x)
{
    if (x == 0) return true;
    int prev = (int)(x & 1);
    int cnt = 1;
    x >>= 1;
    while (x) {
        int cur = (int)(x & 1);
        if (cur == prev) {
            if (++cnt >= 3) return false;
        } else {
            cnt = 1;
            prev = cur;
        }
        x >>= 1;
    }
    return true;
}

static uint64_t A357522_A056539(uint64_t m)
{
    if (m == 0) return 0;
    int bitlen = 64 - __builtin_clzll(m);
    int runs = 1;
    int prev = (int)((m >> (bitlen - 1)) & 1);
    for (int i = bitlen - 2; i >= 0; --i) {
        int b = (int)((m >> i) & 1);
        if (b != prev) { runs++; prev = b; }
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
    /* x = A063037(1+n): the n-th valid number counting from index 0 */
    uint64_t x = 0;
    uint64_t found = 0;
    while (1) {
        if (A357522_ok(x)) {
            if (found == (uint64_t)n) break;
            found++;
        }
        x++;
    }
    uint64_t y = A357522_A056539(x);
    /* k = number of valid values below y */
    uint64_t k = 0;
    for (uint64_t z = 0; z < y; ++z)
        if (A357522_ok(z)) k++;
    return (seq_t)k;
}
