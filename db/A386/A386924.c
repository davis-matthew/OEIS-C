/*
 * A386924  a(n) = denominator(Sum_{k=1..n} d(k)/d(k+1)), where d is the
 *          number of divisors function.
 * https://oeis.org/A386924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the sum_{k=1..n} d(k)/d(k+1), where d(m)
 *            is the number of divisors of m
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 2660, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of the running sum */
    seq_t den = 1;   /* denominator of the running sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t dk  = (seq_t)divisor_count_u64(k);
        seq_t dk1 = (seq_t)divisor_count_u64(k + 1);

        /* new fraction: num/den + dk/dk1  */
        seq_t newnum = num * dk1 + den * dk;
        seq_t newden = den * dk1;

        /* reduce fraction */
        seq_t g = gcd(newnum, newden);
        num = newnum / g;
        den = newden / g;
    }

    /* denominator of the fully reduced sum */
    return den;
}
