/*
 * A371384  a(n) is the denominator of the arithmetic mean of the digits of n.
 * https://oeis.org/A371384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac
 * Formula:   a(n) is the denominator of the arithmetic mean of the decimal
 *            digits of n, i.e. a(n) = count_digits(n) / gcd(sum_digits(n),
 *            count_digits(n))
 * Range:     u8 n <= 2682, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9964 (2026-11-01)
 * Helpers:   digit_sum, gcd, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/gcd.h"
#include "../utils/num_digits0.h"

/* Return the denominator of the reduced fraction
   (sum of decimal digits of n) / (number of decimal digits of n). */
seq_t a(idx_t n)
{
    /* Sum of decimal digits of n. */
    seq_t sum = digit_sum((seq_t)n);

    /* Number of decimal digits of n (always ≥ 1). */
    seq_t cnt;
    if (n == 0)
        cnt = 1;                     /* 0 has one digit "0" */
    else
        cnt = (seq_t)num_digits0((uint64_t)n);

    /* Reduce the fraction.  gcd(0, x) == x, so this also works for n = 0. */
    seq_t g  = gcd(sum, cnt);
    return cnt / g;
}
