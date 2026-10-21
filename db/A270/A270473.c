/*
 * A270473  Expansion of g.f. (1-5*x)/(1-9*x).
 * https://oeis.org/A270473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 4 * 9^(n-1) for n > 0 Recurrence: a(0) = 1,
 *            a(1) = 4, a(n) = 9 * a(n-1) for n > 1 (Both formulas produce the
 *            same sequence.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 0‑based.  For n == 0 we return the special first term. */
    if (n == 0)
        return (seq_t)1;

    /* exp = n-1 is the exponent of 9.  n >= 1 here, so exp is valid. */
    idx_t exp = n - 1;

    /* Fast exponentiation: compute 9^exp modulo the type's full range.
       The operation uses binary exponentiation which is O(log(exp)). */
    seq_t result = 1;          /* multiplicative identity */
    seq_t base    = 9;         /* 9 as seq_t */

    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    /* Multiply by 4 to obtain a(n) = 4 * 9^(n-1). */
    return (seq_t)4 * result;
}
