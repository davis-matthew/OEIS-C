/*
 * A385753  Integer part of the greedy base sqrt(Pi) representation of n.
 * https://oeis.org/A385753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer obtained from the greedy expansion of n
 *            in base √π (digits 0/1)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if desired */

seq_t a(idx_t n)
{
    /* sqrt(pi) as a long-double constant (80-bit precision) */
    const long double BASE = 1.77245385090551602732578070799886L;

    if (n == 0)                /* not part of the offset, but keeps
                                  the function safe for accidental
                                  use with n=0 */
        return (seq_t)0;

    /* Work in long-double to evaluate the real powers of BASE. */
    long double rem = (long double)n;

    /* Find the highest power BASE^k <= n. */
    long double power = 1.0L;
    int k = 0;
    while (power * BASE <= rem) {
        power *= BASE;
        ++k;
    }

    /* Greedy construction of the digits d_k … d_0. */
    seq_t result = 0;          /* decimal number whose digits are the
                                  expansion digits. */

    long double cur = power;   /* current power = BASE^i. */
    for (int i = k; i >= 0; --i) {
        int digit = (rem >= cur) ? 1 : 0;   /* digit ∈ {0,1} */

        /* Append the digit to the decimal result. */
        result = result * 10 + digit;

        if (digit)                   /* subtract the chosen power. */
            rem -= cur;

        if (i > 0)
            cur /= BASE;             /* step down to the next lower power. */
    }

    return result;
}
