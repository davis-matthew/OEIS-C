/*
 * A289979  Define two sequences n1(i) and n2(i) by the recurrences n1(i) =
 *          n1(i-1) + digsum(n2(i-1)), n2(i) = n2(i-1) + digsum(n1(i-1)), with
 *          initial values n1(1) = n and n2(1) = 0. Then a(n) is the smallest
 *          m such that n1(i) = n2(i) = m for some i, or -1 if no such m
 *          exists.
 * https://oeis.org/A289979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the minimal m such that the two sequences * n1(i) and
 *            n2(i) defined by * n1(1) = n, n2(1) = 0 * n1(i) = n1(i-1) +
 *            digit_sum(n2(i-1)) * n2(i) = n2(i-1) + digit_sum(n1(i-1)) * meet
 *            (i.e., n1(i) = n2(i) = m). If they never meet, a(n) = -1. * The
 *            function simulates that chase until equality o...
 * Range:     u8 n <= 24, u16 n <= 75, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include "../utils/digit_sum.h"

#define MAX_STEPS ((size_t)1000000)   /* safety bound to avoid infinite loops */

seq_t a(idx_t n)
{
    seq_t m1 = (seq_t)n;   /* n1(1) */
    seq_t m2 = (seq_t)0;   /* n2(1) */
    size_t steps = 0;

    while (steps < MAX_STEPS) {
        if (m1 == m2)
            return m1;

        /* compute digit sums of current values */
        seq_t d1 = digit_sum(m1);
        seq_t d2 = digit_sum(m2);

        /* advance both sequences */
        m1 += d2;
        m2 += d1;

        ++steps;
    }

    /* If we exceed the step limit, conclude no meeting point */
    return (seq_t)(-1);
}
