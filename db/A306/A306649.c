/*
 * A306649  a(n) = numerator of Sum_{d|n} (d/sigma(d)) where sigma(k) = the
 *          sum of the divisors of k (A000203).
 * https://oeis.org/A306649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} (d / sigma(d))
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            where sigma(m) = sum of the divisors of m.              */
/* The implementation iterates over all divisors of n, computes     */
/* sigma(d) by a simple divisor loop, and keeps the fraction in its  */
/* lowest‑term form at each step using the provided gcd routine.      */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the sum of divisors of m (sigma(m)) using only uint64_t.   */
/* This routine is fast enough for the small values needed here.     */
static seq_t sigma_uint64(uint64_t m)
{
    seq_t sum = 0;
    uint64_t i;
    for (i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            sum += (seq_t)i;
            uint64_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* safety; offset starts at 1           */
        return 0;

    seq_t sum_num = 0;        /* current numerator of the reduced sum */
    seq_t sum_den = 1;        /* current denominator                 */

    uint64_t N = (uint64_t)n;
    uint64_t d;
    for (d = 1; d <= N; ++d) {
        if (N % d != 0)
            continue;        /* skip non‑divisors                   */

        seq_t term_num = (seq_t)d;          /* numerator of the new term   */
        seq_t term_den = sigma_uint64(d);   /* denominator of the new term */

        /* Reduce the new denominator against the current one. */
        seq_t g = gcd(sum_den, term_den);
        seq_t a = sum_den / g;              /* sum_den   part without g   */
        seq_t b = term_den / g;             /* term_den  part without g   */

        /* New numerator/denominator before final reduction. */
        seq_t new_num = sum_num * b + term_num * a;
        seq_t new_den = a * term_den;       /* equivalently (sum_den/g)*term_den */

        /* Reduce the fraction again. */
        seq_t g2 = gcd(new_num, new_den);
        sum_num = new_num / g2;
        sum_den = new_den / g2;
    }

    return sum_num;        /* numerator of the completely reduced sum  */
}
