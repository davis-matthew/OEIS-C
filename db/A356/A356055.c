/*
 * A356055  Intersection of A001952 and A137804.
 * https://oeis.org/A356055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer m that is in both A001952
 *            (floor(k*(2+sqrt(2)))) and A137804 (floor(j*(4*sqrt(2)+9)/7));
 *            found by scanning A001952 terms and testing membership in
 *            A137804
 * Range:     u8 none, u16 n <= 6, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

static inline seq_t A356055_isqrt(seq_t x)
{
    if (x == 0) return 0;
    unsigned bits = bit_length(x);
    seq_t low  = 0;
    seq_t high = ((seq_t)1) << ((bits + 1) / 2);
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t mid_sq = mid * mid;
        if (mid_sq <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

static seq_t A356055_A001952(seq_t k)
{
    return k + k + A356055_isqrt(2 * k * k);
}

static seq_t A356055_A137804(seq_t N)
{
    seq_t A = 9 * N;
    seq_t B = 4 * N;
    seq_t left = B * B * 2;
    seq_t b = A356055_isqrt(left);
    seq_t base = A + b;
    seq_t q0 = base / 7;
    seq_t m_value = (q0 + 1) * 7 - A;
    int add_one = 0;
    if (m_value <= 0) {
        add_one = 1;
    } else {
        seq_t div = left / m_value;
        if (div >= m_value) add_one = 1;
    }
    return q0 + add_one;
}

static int A356055_in_A137804(seq_t m)
{
    seq_t x = A356055_isqrt(32 * m * m);
    seq_t j0 = (9 * m - x) / 7;
    seq_t j = (j0 > 0) ? j0 - 1 : 0;
    seq_t jend = j0 + 3;
    for (; j <= jend; j++) {
        seq_t v = A356055_A137804(j);
        if (v == m) return 1;
        if (v > m) return 0;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;
    seq_t m = 0;
    while (count < n) {
        k++;
        m = A356055_A001952(k);
        if (A356055_in_A137804(m)) count++;
    }
    return m;
}
