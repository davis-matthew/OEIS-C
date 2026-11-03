/*
 * A275626  Denominator of Sum_{k=1..n} 0.k.
 * https://oeis.org/A275626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = denominator of the sum \sum_{k=1}^n (k / 10^{digits(k)})
 * Range:     u8 n <= 9, u16 n <= 444, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 89 terms through n = 8730 (2026-11-01)
 * Helpers:   gcd, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t num = 0;          /* current numerator of the sum   */
    seq_t den = 1;          /* current denominator of the sum */

    for (idx_t k = 1; k <= n; ++k) {
        /* Current term: k / 10^{digits(k)}   */
        seq_t termNum = (seq_t)k;
        seq_t termDen = pow10(num_digits0((uint64_t)k));   /* 10^digits(k) */

        /* Compute gcd of the two denominators */
        seq_t g = gcd(den, termDen);

        /* lcm = den/g * termDen   */
        seq_t lcm = (den / g) * termDen;

        /* Add the fractions using lcm as common denominator */
        seq_t newNum = num * (termDen / g) + termNum * (den / g);
        seq_t newDen = lcm;

        /* Reduce the fraction by its gcd */
        seq_t g2 = gcd(newNum, newDen);
        num = newNum / g2;
        den = newDen / g2;
    }

    return den;   /* denominator of the sum up to n */
}
