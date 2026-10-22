/*
 * A352153  Smallest digit in the decimal expansion of 1/n, ignoring leading
 *          and trailing 0's.
 * https://oeis.org/A352153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest non‑leading digit in the decimal expansion of 1/n
   (ignoring leading zeros and trailing zeros).  The function works for
   any n ≥ 1.  The algorithm is a simple long division, iterating up
   to n digits after the first non‑zero digit (the fractional period
   is always ≤ n−1). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;      /* 1/1 = 1 */
    if (n == 3) return 3;      /* 1/3 = 0.333… */

    idx_t r = 1 % n;           /* current remainder */
    seq_t min_digit = 9;       /* worst possible digit  */
    bool started = false;      /* have we seen a non‑zero? */

    /* Find first non‑zero digit to skip leading zeros */
    while (true) {
        r *= 10;
        seq_t d = r / n;       /* current digit */
        r %= n;

        if (!started) {
            if (d != 0) {      /* first non‑zero digit found */
                started = true;
                min_digit = d;
                break;
            }
            /* otherwise still leading zero – skip */
        }

        if (r == 0) break;     /* terminating decimal */
    }

    /* All digits were zeros – impossible for n ≥ 2, but guard anyway */
    if (!started) return min_digit;

    /* Process the rest of the digits up to the period length. */
    for (idx_t i = 0; i < n; ++i) {
        if (r == 0) break;     /* terminating decimal */
        r *= 10;
        seq_t d = r / n;
        r %= n;
        if (d < min_digit) min_digit = d;
        if (min_digit == 0) break;  /* cannot get smaller */
    }

    return min_digit;
}
