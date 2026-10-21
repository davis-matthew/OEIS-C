/*
 * A277291  Largest number that is n times sum of its digits when written in
 *          base 5.
 * https://oeis.org/A277291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2852, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   digit_count5, sum_digits5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277291: largest number that is n times the sum of its base‑5 digits */
/* For each n ≥ 0 return the largest integer x with x = n * sum_5(x) (0 if none) */

#include <stddef.h>   /* for size_t (used only for array indices) */
#include "../utils/digit_count5.h"
#include "../utils/sum_digits5.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0) return 0;

    seq_t N    = (seq_t)n;   /* promote to seq_t to avoid overflow in multiplications   */
    seq_t best = 0;          /* keep the largest found value */

    /* iterate over possible digit sums s.  For each s compute x = N*s
       and check whether sum_digits5(x) == s.  If the condition holds,
       we record x as a candidate.  We stop once s exceeds the maximum
       attainable digit sum for the current number of digits.  The loop
       is finite because, asymptotically, s grows linearly while the
       bound 4*digits(x) grows only logarithmically. */
    for (seq_t s = 1; ; ++s) {
        seq_t x   = N * s;
        seq_t sum = sum_digits5(x);

        if (sum == s) {
            /* x satisfies the equation; remember the largest such value. */
            best = x;
        }

        /* Compute the digit count of x to test the stop condition. */
        seq_t digits = digit_count5(x);

        /* If the candidate sum s is already larger than the maximum
           possible digit sum for its number of digits, no larger s
           can satisfy the equation. */
        if (s > 4 * digits) {
            break;
        }
    }

    return best;
}
