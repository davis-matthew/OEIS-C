/*
 * A329537  a(n) is the minimum number whose product of digits contains n
 *          digits.
 * https://oeis.org/A329537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329537:  a(n) = minimal integer whose product of digits has at least
 * n decimal digits.  The algorithm finds the minimal length L of the
 * number (using only digits 2–9) and then constructs the lexicographically
 * smallest sequence of digits that meets the product requirement.  The
 * construction uses logarithms to test feasibility while keeping all
 * intermediate arithmetic in the native integer type seq_t. */

#include <stdint.h>
#include <math.h>

/* Pre‑computed decimal logarithms for digits 0–9.  Digits 0 and 1 are
 * unused; the value for 0 is -INFINITY and for 1 is 0.0. */
static const long double LOGDIG[10] = {
    -INFINITY,                /* 0   not used */
    0.0L,                     /* 1   not used */
    0.3010299956639812L,      /* 2   */
    0.47712125471966244L,     /* 3   */
    0.6020599913279624L,      /* 4   */
    0.6989700043360189L,      /* 5   */
    0.7781512503836436L,      /* 6   */
    0.8450980400142568L,      /* 7   */
    0.903089987991943L,       /* 8   */
    0.9542425094393249L       /* 9   */
};

/* Maximum possible length for n up to 10000 matches < 12 000; 20 000
 * gives a comfortable buffer.  The digits array can be allocated
 * statically because the function is pure (no side effects). */
static uint8_t digits[20000];

seq_t a(idx_t n)
{
    if (n == 1) /* a(1)=1 */
        return 1U;

    const long double log9      = LOGDIG[9];
    const long double targetLog = (long double)(n - 1);          /* log10(10^(n-1)) */

    /* Compute minimal required length L: ceil(targetLog / log9). */
    idx_t L = (idx_t)(targetLog / log9);
    if ((long double)L + 1e-12L < targetLog / log9)
        ++L;

    /* Construct the smallest digit sequence of length L satisfying the
     * product condition.  curLog stores the log10 of the product of the
     * digits chosen so far. */
    long double curLog = 0.0L;
    for (idx_t pos = 0; pos < L; ++pos)
    {
        for (int d = 2; d <= 9; ++d)   /* digits 2–9 */
        {
            long double newLog = curLog + LOGDIG[d]
                               + (long double)(L - pos - 1) * log9;
            if (newLog + 1e-12L >= targetLog)   /* feasible continuation */
            {
                digits[pos] = (uint8_t)d;
                curLog     += LOGDIG[d];
                break;
            }
        }
    }

    /* Build the result as an integer number from the digit sequence. */
    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i)
        result = result * 10 + digits[i];

    return result;
}
