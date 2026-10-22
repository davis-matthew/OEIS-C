/*
 * A381770  a(n) is the least k > 0 such that the factorial base expansion of
 *          k*n has digits in nonincreasing order.
 * https://oeis.org/A381770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 4235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorial_digits_nonincreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A381770:
    a(n) = least k > 0 such that the factorial base expansion of k*n
    has digits in nonincreasing order (read from most significant to
    least significant).  For n = 0 the first term of the sequence is 1.  */

/*  The code uses only integer arithmetic on seq_t and idx_t, and
    works with the arbitrary‑precision build as well as 64‑bit builds.  */

#include <stdbool.h>
#include "../utils/factorial_digits_nonincreasing.h"

seq_t a(idx_t n)
{
    /*  Offset 0: a(0) = 1  */
    if (n == 0)
        return 1;

    /*  Convert n to seq_t once for efficiency.  */
    seq_t sn = (seq_t)n;

    /*  Search the minimal positive k.  */
    for (seq_t k = 1;; ++k) {
        if (factorial_digits_nonincreasing(sn * k))
            return k;
    }
}
