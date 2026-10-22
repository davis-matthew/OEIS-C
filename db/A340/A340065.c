/*
 * A340065  Decimal expansion of the Product_{p>=2} 1+p^2/((p-1)^2*(p+1)^2)
 *          where p are successive prime numbers A000040.
 * https://oeis.org/A340065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   A340065 – the decimal digits of the rational 5005/2764 (period
 *            length 230, integer part 1, first two decimals non‑repeating)
 * Range:     u8 none, u16 n <= 105, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1 integer digit + 2 preperiod + 230 period = 233 digits */
    static seq_t digits[233];
    static bool init = false;
    if (!init)
    {
        const seq_t num = (seq_t)5005;   /* numerator */
        const seq_t den = (seq_t)2764;   /* denominator */

        /* integer part */
        digits[0] = num / den;
        seq_t rem = num % den;
        size_t pos = 1;

        /* track seen remainders to detect period */
        int seen[2765];
        for (int i = 0; i <= 2764; ++i) seen[i] = -1;

        while (rem != 0 && pos < 233)
        {
            int r = (int)rem;            /* rem < 2764 */
            if (seen[r] != -1) break;    /* remainder repeats → period found */

            seen[r] = (int)pos;
            rem *= (seq_t)10;
            digits[pos++] = rem / den;
            rem = rem % den;
        }

        init = true;
    }

    /* Index handling: a(1) is integer digit, others are decimal digits. */
    if (n == 1)            return digits[0];
    if (n == 2)            return digits[1];
    if (n <= 233)          return digits[n - 1];

    /* For n > 233, the decimal part repeats with period length 230
       starting at digits[3]. */
    size_t idx = 3 + ((n - 234) % 230);
    return digits[idx];
}
