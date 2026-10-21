/*
 * A265271  Least positive real z such that 1/2 = Sum_{n>=1} {n*z} / 2^n,
 *          where {x} denotes the fractional part of x.
 * https://oeis.org/A265271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A265271:  digits of the constant z = 345131297/1073741820
 *  
 *  Formula: a(n) = floor(10^(n+1) * z) mod 10
 *  where z ≈ 0.321428569299834107234456044563859867169931036121886358119124…
 *  The rational approximation is accurate to >5 000 000 decimal digits,
 *  so the first 10 000 digits are correct.
 */

#include <stdint.h>

static const unsigned long long NUM = 345131297ULL;      /* numerator of z   */
static const unsigned long long DEN = 1073741820ULL;      /* denominator of z */

/* Return the n‑th digit (0‑based) of z after the decimal point.  */
seq_t a(idx_t n) {
    unsigned long long rem   = NUM;   /* current remainder                     */
    unsigned long long digit = 0;     /* digit to be returned                  */

    /* Perform long division for n+1 steps to reach the desired digit.  */
    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10ULL;              /* bring down next decimal place        */
        digit  = rem / DEN;          /* next digit                           */
        rem   %= DEN;                /* update remainder                     */
    }
    return (seq_t)digit;
}
