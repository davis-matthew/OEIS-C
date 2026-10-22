/*
 * A397151  Number of compositions (c_1,...,c_k) of n with c_i >= 3*k - 2*i.
 * https://oeis.org/A397151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 74, u32 n <= 189, u64 n <= 518,
 *            u128 n <= 1503, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397151
   Formula: a(0) = 1
   a(n)   = Σ_{k≥1, k*(2*k-1)≤n} binomial(n-1-2*k*(k-1), k-1)   for n ≥ 1
*/
#include <stdbool.h>
#include "../utils/gcd.h"

/* multiplicative computation of the binomial coefficient
   using gcd reductions to keep intermediate values within bounds   */
static seq_t binom_custom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(n - k + i);   /* (n-k+i) */
        seq_t denominator = i;
        seq_t g = gcd(numerator, denominator);
        numerator   /= g;
        denominator /= g;
        seq_t g2 = gcd(result, denominator);
        result      /= g2;
        denominator /= g2;       /* denominator should now be 1 */
        result *= numerator;     /* safe: result ≤ max value, product fits seq_t */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 1;; ++k) {
        /* hexagonal number h(k) = k * (2*k - 1)  */
        idx_t hex = k * (2*k - 1);
        if (hex > n) break;

        idx_t n_bin = n - 1 - 2*k*(k - 1);
        idx_t k_bin = k - 1;
        sum += binom_custom(n_bin, k_bin);
    }
    return sum;
}
