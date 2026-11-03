/*
 * A353295  Square nearest to the sum of the first n positive squares.
 * https://oeis.org/A353295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest square to n(n+1)(2n+1)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 2342, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 61 terms through n = 9531 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sum = n_seq * (n_seq + 1) * (2 * n_seq + 1) / 6;

    /* nearest square */
    seq_t sq = floor_sqrt(sum);          /* floor(sqrt(sum))          */
    seq_t sq2 = sq * sq;                 /* sq^2                      */
    seq_t nxt = sq + 1;
    seq_t nxt2 = nxt * nxt;              /* (sq+1)^2                  */

    seq_t diff1 = sum - sq2;             /* sum - sq^2                */
    seq_t diff2 = nxt2 - sum;            /* (sq+1)^2 - sum            */

    return (diff1 <= diff2) ? sq2 : nxt2;
}
