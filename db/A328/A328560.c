/*
 * A328560  Numbers whose product of digits is a power of 10.
 * https://oeis.org/A328560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 166, u32 n <= 8549, u64 n <= 8549,
 *            u128 n <= 8549, big any n
 * Helpers:   build_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose product of digits is a power of 10.
   This function enumerates numbers using only the digits 1,2,4,5,8 in
   ascending numeric order and returns the n‑th one whose digit product
   equals 10^k for some k ≥ 0. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_digits.h"

seq_t a(idx_t n)
{
    idx_t  count  = 0;   /* how many valid numbers seen so far   */
    seq_t  result = 0;   /* value to return when `n` is reached */
    bool   found  = false;

    for (size_t len = 1; !found; ++len) {
        build_digits(0, len, (seq_t)0, 0, 0, n, &count, &result, &found);
    }
    return result;
}
