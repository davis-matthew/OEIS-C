/*
 * A245446  Permutation of nonnegative integers: a(n) = A193231(A056539(n)).
 * https://oeis.org/A245446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A193231(A056539(n))
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 86 terms through n = 1005 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A193231_a(idx_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;
    idx_t t = n;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

static idx_t A056539_a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t m = (uint64_t)n;
    int bitlen = 64 - __builtin_clzll(m);
    int run_count = 1;
    int prev_bit = (int)((m >> (bitlen - 1)) & 1);
    for (int i = bitlen - 2; i >= 0; --i) {
        int bit = (int)((m >> i) & 1);
        if (bit != prev_bit) {
            ++run_count;
            prev_bit = bit;
        }
    }
    uint64_t rev = 0;
    for (int i = 0; i < bitlen; ++i)
        rev = (rev << 1) | ((m >> i) & 1);
    if ((run_count & 1) == 0) {
        uint64_t mk = (bitlen == 64) ? ~(uint64_t)0 : (((uint64_t)1 << bitlen) - 1);
        rev ^= mk;
    }
    return (idx_t)rev;
}

seq_t a(idx_t n)
{
    return A193231_a(A056539_a(n));
}
