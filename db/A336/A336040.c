/*
 * A336040  Characteristic function of refactorable numbers (A033950).
 * https://oeis.org/A336040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n mod τ(n) == 0] (characteristic function of
 *            refactorable numbers)
 * Range:     u8 n <= 1251, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; safety for n==0 */

    /* τ(n) – number of divisors of n */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* a(n) = 1 iff n is divisible by d, else 0 */
    return ((seq_t)n % d == 0) ? 1 : 0;
}
