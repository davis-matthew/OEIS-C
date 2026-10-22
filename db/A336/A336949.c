/*
 * A336949  a(n) = n! * [x^n] 1 / (exp(-n*x) - x).
 * https://oeis.org/A336949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n*(n-k+1))^k / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n! */
    seq_t fact_n = factorial(n);
    /* factor == n! / k! for the current value of k */
    seq_t factor = fact_n;
    seq_t total  = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow_val;
        if (k == 0)
        {
            pow_val = 1;
        }
        else
        {
            seq_t base = (seq_t)n * ((seq_t)n - (seq_t)k + 1);
            pow_val = big_pow(base, (seq_t)k);
        }

        seq_t term = pow_val * factor;
        total += term;

        if (k < n)
        {
            factor /= (seq_t)(k + 1);
        }
    }

    return total;
}
