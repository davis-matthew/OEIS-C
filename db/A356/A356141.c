/*
 * A356141  a(n) = A137804(A001952(n)).
 * https://oeis.org/A356141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A137804(A001952(n)), where A001952(n) =
 *            floor(n*(2+sqrt(2))) and A137804(m) = floor(m*(4*sqrt(2)+9)/7)
 * Range:     u8 none, u16 n <= 13, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

static inline seq_t A356141_isqrt(seq_t x)
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

static seq_t A356141_A001952(seq_t n)
{
    return n + n + A356141_isqrt(2 * n * n);
}

static seq_t A356141_A137804(seq_t N)
{
    seq_t A = 9 * N;
    seq_t B = 4 * N;
    seq_t left = B * B * 2;
    seq_t b = A356141_isqrt(left);
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

seq_t a(idx_t n)
{
    return A356141_A137804(A356141_A001952((seq_t)n));
}
