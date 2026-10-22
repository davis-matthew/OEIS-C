/*
 * A390648  E.g.f. A(x) satisfies A(x) = 1 + x^3*exp(x)*A(x)^2.
 * https://oeis.org/A390648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^(n-3*k) *
 *            Catalan(k)/(n-3*k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   catalan, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t exp = n - 3 * k;

        /* product = n * (n-1) * ... * (n-3k+1)  (empty product is 1) */
        seq_t prod = 1;
        for (idx_t i = 0; i < 3 * k; ++i)
            prod *= (seq_t)(n - i);

        /* term = prod * Catalan(k) * k^exp */
        seq_t term = prod * catalan(k);
        term *= seq_pow((seq_t)k, exp);

        sum += term;
    }

    return sum;
}
