/*
 * A265026  First differences of A048701.
 * https://oeis.org/A265026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048701(n) - A048701(n-1) A048701(n) is the n-th binary
 *            palindrome of even length (sorted by value). a(1) = 3 (the first
 *            such palindrome)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8039, u64 n <= 8039,
 *            u128 n <= 8039, big any n
 * Helpers:   nth_pal_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_pal_even.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, return 0 as safe value */

    if (n == 1)
        return nth_pal_even(1);   /* a(1) = 3 */

    seq_t cur  = nth_pal_even(n);
    seq_t prev = nth_pal_even(n - 1);
    return cur - prev;
}
