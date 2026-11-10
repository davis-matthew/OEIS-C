/*
 * A328973  Numbers k such that A053392(k) > k.
 * https://oeis.org/A328973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A053392(k) > k; found by scanning k
 *            upward and comparing the digit string of A053392(k) with that of
 *            k
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A328973_greater(uint64_t k)
{
    if (k < 10) return false;
    int d[24];
    int len = 0;
    uint64_t t = k;
    while (t > 0) { d[len++] = (int)(t % 10); t /= 10; }
    /* d[len-1] is most significant */
    int r[48];
    int rl = 0;
    for (int i = len - 1; i >= 1; --i) {
        int s = d[i] + d[i - 1];
        if (s >= 10) { r[rl++] = s / 10; r[rl++] = s % 10; }
        else r[rl++] = s;
    }
    if (rl != len) return rl > len;
    for (int i = 0; i < len; ++i) {
        int kd = d[len - 1 - i];
        if (r[i] != kd) return r[i] > kd;
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 9;
    while (count < (uint64_t)n) {
        ++k;
        if (A328973_greater(k)) ++count;
    }
    return (seq_t)k;
}
