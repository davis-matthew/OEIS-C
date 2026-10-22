/*
 * A371095  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(1, k) = 8*k-7, and A(n+1, k) =
 *          R(A(n, k)), n,k >= 1, where Reduced Collatz function R(n) gives
 *          the odd part of 3n+1.
 * https://oeis.org/A371095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   odd_part_of_three_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371095: read A(n,k) upward antidiagonally.
   A(1, k) = 8*k – 7
   A(n+1, k) = odd_part(3*A(n, k)+1),  where odd_part(x) is the odd part of x. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/odd_part_of_three_plus_one.h"

/* find minimal m such that m*(m+1)/2 >= n.  Uses binary search with 128‑bit
   arithmetic so no overflow occurs for any 64‑bit n. */
static inline idx_t find_m(idx_t n)
{
    idx_t low = 0, high = 1;
    while (( (__uint128_t)high * (high + 1) / 2 ) < n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__uint128_t)mid * (mid + 1) / 2 ) >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t m = find_m(n);
    /* previous triangular number m*(m-1)/2 */
    idx_t prev = (idx_t)(((__uint128_t)m * (m - 1)) / 2);
    idx_t p = n - prev;          /* 1‑based position in diagonal */
    idx_t row = m - p + 1;
    idx_t col = p;

    seq_t val = (seq_t)((seq_t)8 * (seq_t)col - 7);
    for (idx_t r = 2; r <= row; ++r)
        val = odd_part_of_three_plus_one(val);
    return val;
}
