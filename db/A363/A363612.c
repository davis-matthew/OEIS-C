/*
 * A363612  Number of iterations of phi(x) at n needed to reach a square.
 * https://oeis.org/A363612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of φ(x) until x becomes a perfect
 *            square
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t r = floor_sqrt(x);
    if (r * r == x) {
        return 0;
    }
    seq_t cnt = 0;
    while (true) {
        x = totient_seq(x);
        ++cnt;
        r = floor_sqrt(x);
        if (r * r == x) {
            return cnt;
        }
    }
}
