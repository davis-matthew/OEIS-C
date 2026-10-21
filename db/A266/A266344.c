/*
 * A266344  a(n) = number of ways n can be divided into two factors that have
 *          the same number of digits in factorial base representation (the
 *          two different orders for unequal factors are counted only once).
 * https://oeis.org/A266344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, d <= n/d} [ A084558(d) = A084558(n/d) ]
 *            A084558(x) is the number of digits in factorial-base
 *            representation of x. Digits are counted by the greedy algorithm
 *            where the k‑th coefficient is (x / (k+1)) % (k+2). The length of
 *            the representation is the highest k for which the coeffici...
 * Range:     u8 n <= 9438, u16 n <= 9438, u32 n <= 9438, u64 n <= 9438,
 *            u128 n <= 9438, big any n
 * Helpers:   factorial_digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial_digit_count.h"

/* Count factor pairs with the same factorial-base digit length. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset starts at 1, but guard anyway */

    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (factorial_digit_count(d) == factorial_digit_count(e))
                ++count;
        }
    }
    return count;
}
