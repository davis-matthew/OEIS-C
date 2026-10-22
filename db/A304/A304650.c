/*
 * A304650  Number of ways to write n as a product of two positive integers,
 *          neither of which is a perfect power.
 * https://oeis.org/A304650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (a,b) with a*b = n, * where
 *            neither a nor b is a perfect power
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  © 2026.  The definitions of idx_t and seq_t are provided by the
 *   build system; this code uses only integer arithmetic.            */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Test whether x is a perfect power (m^k with k ≥ 2, m ≥ 1).
 * 1 is regarded as a perfect power. 0 is not.                      */
static bool is_perfect_power(idx_t x)
{
    if (x == 0)          /* 0 is not a perfect power */
        return false;
    if (x == 1)          /* 1 = 1^k for any k ≥ 2 */
        return true;

    /* Try all bases m ≥ 2.  If m^2 > x we can stop.            */
    for (idx_t m = 2; m * m <= x; ++m) {
        idx_t power = m * m;           /* m^2          */
        while (power <= x) {
            if (power == x)          /* exact match   */
                return true;
            /* Next power: avoid overflow using division.     */
            if (power > x / m)        /* would overflow */
                break;
            power *= m;               /* m^(k+1)       */
        }
    }
    return false;
}

/*  Main function required by the build system.                         */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    seq_t count = 0;

    /* Enumerate divisor pairs (d, e) with d <= e.
     * Using n / d avoids overflow in the loop condition.           */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;       /* d not a divisor */

        idx_t e = n / d;
        if (is_perfect_power(d) || is_perfect_power(e))
            continue;       /* at least one factor is a perfect power */

        if (d == e)
            ++count;        /* (d,d) counted once */
        else
            count += 2;     /* (d,e) and (e,d) */
    }

    return count;
}
