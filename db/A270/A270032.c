/*
 * A270032  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 6 (or 0 if no such base exists).
 * https://oeis.org/A270032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest base b ≥ 7 such that the base‑b representation
 * of n contains the digit 6; returns 0 if no such base exists.  By
 * construction a(1)–a(5) = 0, a(6) = 7, and for all n ≥ 13 a(n) > 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1–5: no base  ≥ 7 can contain digit 6 for these n */
    if (n < 6U)
        return 0;

    /* n == 6 is represented as "6" in base 7 */
    if (n == 6U)
        return 7;

    /* Search bases from 7 up to n‑1; for b == n the representation is the
     * single digit n, which cannot be 6 unless n == 6 (handled above). */
    for (idx_t b = 7; b < n; ++b)
    {
        idx_t temp = n;
        while (temp > 0)
        {
            idx_t rem = temp % b;
            if (rem == 6)
                return (seq_t)b;
            temp /= b;
        }
    }

    /* If no base satisfies the condition, return 0. */
    return 0;
}
