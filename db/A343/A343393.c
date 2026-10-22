/*
 * A343393  Decimal expansion of 99^2/1103, an approximation to 2*Pi*sqrt(2)
 *          from Ramanujan.
 * https://oeis.org/A343393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 99^2/1103 (integer part first)
 * Range:     u8 none, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period 1102.  The sequence digits are obtained by the
 * standard long‑division algorithm: divide 9801 by 1103, then repeatedly
 * multiply the remainder by 10, take the quotient as the next digit,
 * and keep the new remainder.  The first element (n = 1) is the integer
 * part of 9801/1103.  This implementation precomputes the 1102‑cycle
 * once and returns a[n] as cache[(n-1) % 1102]. */

#include <stdbool.h>

static seq_t digits[1102];
static bool initialized = false;

static void init_digits(void)
{
    if (initialized)
        return;

    const seq_t num = 9801ULL;   /* 99^2 */
    const seq_t den = 1103ULL;   /* prime divisor */

    /* first digit: integer part */
    digits[0] = num / den;
    /* remainder after extracting integer part */
    seq_t rem = num % den;

    /* remaining 1101 digits of the repeating block */
    for (idx_t i = 1; i < 1102; ++i) {
        rem *= 10ULL;               /* shift remainder */
        digits[i] = rem / den;      /* next decimal digit */
        rem %= den;                 /* new remainder */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n <= 0, but harmless */

    init_digits();
    return digits[(n - 1ULL) % 1102ULL];
}
