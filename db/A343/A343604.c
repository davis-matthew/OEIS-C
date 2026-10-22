/*
 * A343604  a(n) is the least number > n with the same sum of balanced ternary
 *          digits as n.
 * https://oeis.org/A343604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m > n with sum of balanced ternary digits of m
 *            equal to that of n
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: directly computes the sum of balanced ternary digits for a number,
   then scans forward until the same sum is found again. */

#include <stdbool.h>   /* for bool, but not actually used here */
#include "../utils/balanced_digit_sum.h"

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    long long target_sum = balanced_digit_sum((seq_t)n);
    seq_t m = (seq_t)n + 1;

    for (;; ++m)
    {
        if (balanced_digit_sum(m) == target_sum)
        {
            return m;
        }
    }
}
