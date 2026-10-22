/*
 * A373281  Expansion of Sum_{k>=0} x^(5^k) / (1 - 5*x^(5^k)).
 * https://oeis.org/A373281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n-1) + a(n/5) if n mod 5 == 0; otherwise a(n) =
 *            5^(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided power5 utility for efficient 5^exp computation. */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for n=0, but not used; return 0 to avoid UB. */
        return 0;

    seq_t result = power5(n - 1);

    if ((n % 5) == 0)
        result += a(n / 5);

    return result;
}
