/*
 * A330786  Number of steps to reach 1 by iterating the absolute alternating
 *          sum-of-divisors function (A206369).
 * https://oeis.org/A330786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   abs_alt_sum_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0; for n > 1, a(n) = 1 + a(A206369(n)).
   The implementation iteratively applies the absolute alternating
   sum-of-divisors function A206369 until reaching 1. */

#include <stdint.h>
#include "../utils/abs_alt_sum_divisors.h"

/* The sequence function as required by the prompt. */
seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* by definition */

    seq_t steps = 0;
    idx_t cur = n;

    while (cur != 1) {
        seq_t f = abs_alt_sum_divisors(cur);

        /* Convert f back to idx_t for the next iteration. For
           C++ compilation we use static_cast, which is also
           acceptable in C when the class has a suitable
           conversion operator. */
#ifdef __cplusplus
        cur = static_cast<idx_t>(f);
#else
        cur = (idx_t)f;
#endif

        steps++;
    }

    return steps;
}
