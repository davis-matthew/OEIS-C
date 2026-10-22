/*
 * A351737  Expansion of e.g.f. exp( x * (exp(3 * x) - 1) ).
 * https://oeis.org/A351737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3*k-1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute all terms for k >= 1 first, then add the special k = 0 term
       which is (-1)^n.  Adding 1 or subtracting 1 afterwards keeps the
       running sum unsigned (the intermediate sum is always >= 1 for odd n). */
    seq_t total = 0;          /* sum for k = 1 .. n */
    seq_t c      = 1;         /* binomial coefficient C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* update c to C(n,k) */
        c = c * (seq_t)(n - k + 1) / (seq_t)k;

        seq_t base = (seq_t)(3 * k - 1);
        seq_t exp  = (seq_t)(n - k);      /* exponent is nonnegative */
        seq_t term = big_pow(base, exp);
        term = term * c;
        total += term;
    }

    /* add the k = 0 contribution (-1)^n */
    if (n % 2 == 0) {
        total += 1;     /* (+1) for even n */
    } else {
        total -= 1;     /* (-1) for odd n; total >= 1 here */
    }

    return total;
}
