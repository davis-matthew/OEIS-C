/*
 * A329123  Row sums of A329060.
 * https://oeis.org/A329123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A329123
   Formula: a(n) = binomial(3 + 5*n, n) * 2F1([1, -n], [-3-5*n], 1+n) / (1+n)   */
/*  a(0) = 1, a(1) = 5, a(2) = 41, a(3) = 420, ... */

#include <stdbool.h>
#include <assert.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  Hypergeometric sum H(n) = Σ_{k=0}^{n} (-n)_k * (n+1)^k / (-3-5n)_k
        The terms are positive.  We compute the sum as a rational number
        using exact arithmetic, reducing at each step.  */
    seq_t term_num = 1;          /* current term numerator   */
    seq_t term_den = 1;          /* current term denominator */
    seq_t sum_num  = 1;          /* sum numerator            */
    seq_t sum_den  = 1;          /* sum denominator          */
    seq_t y        = (seq_t)n + 1;  /* y = n+1                 */

    for (idx_t k = 0; k < n; ++k) {
        /* compute next term:
           T_{k+1} = T_k * (n - k) * y / (3 + 5*n - k)   */
        seq_t num_factor = (seq_t)n - k;
        seq_t den_factor = (seq_t)3 + (seq_t)5*n - k;

        /* multiply term numerator and denominator */
        term_num = term_num * num_factor;
        term_num = term_num * y;
        term_den = term_den * den_factor;

        /* reduce the current term */
        seq_t g = gcd(term_num, term_den);
        if (g > 1) {
            term_num /= g;
            term_den /= g;
        }

        /* add term to the accumulated sum */
        seq_t g2 = gcd(sum_den, term_den);
        seq_t lcm = sum_den / g2 * term_den;               /* lcm of denominators */
        seq_t part1 = sum_num * (term_den / g2);
        seq_t part2 = term_num * (sum_den / g2);
        seq_t new_sum_num = part1 + part2;
        seq_t new_sum_den = lcm;

        seq_t g3 = gcd(new_sum_num, new_sum_den);
        if (g3 > 1) {
            new_sum_num /= g3;
            new_sum_den /= g3;
        }
        sum_num = new_sum_num;
        sum_den = new_sum_den;
    }

    /* final computation:
       a(n) = binomial(3+5*n, n) * sum_num / sum_den / (n+1)   */
    seq_t binom = binomial((idx_t)3 + (idx_t)5*n, n);

    seq_t den = sum_den * ((seq_t)n + 1);
    seq_t num = binom;

    /* cancel common factors with the denominator before final multiplication */
    seq_t g1 = gcd(num, den);
    if (g1 > 1) {
        num /= g1;
        den /= g1;
    }
    seq_t g2 = gcd(sum_num, den);
    if (g2 > 1) {
        sum_num /= g2;
        den /= g2;
    }
    /* The denominator should now be 1 for the exact integer result. */
    assert(den == 1);

    /* final integer result */
    return num * sum_num;
}
