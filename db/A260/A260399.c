/*
 * A260399  Positions of 1 in the infinite palindromic word at A260397.
 * https://oeis.org/A260399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the word A260397 (A260397:
 *            a(1)=1,a(2)=1,a(3)=0; a(2^k)=a(k-1); otherwise
 *            a(n)=a(2^(floor(log2 n)+1)-n)); computed by scanning
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9268 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t A260399_floor_log2(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

static seq_t A260397_a(idx_t n)
{
    if (n <= 3) {
        static const seq_t base[3] = {1, 1, 0};
        return base[n - 1];
    }
    if ((n & (n - 1)) == 0) {
        idx_t e = A260399_floor_log2(n);
        return A260397_a(e - 1);
    }
    idx_t p = A260399_floor_log2(n);
    idx_t mirror = (idx_t)1 << (p + 1);
    return A260397_a(mirror - n);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        ++m;
        if (A260397_a(m) == 1) ++count;
    }
    return (seq_t)m;
}
