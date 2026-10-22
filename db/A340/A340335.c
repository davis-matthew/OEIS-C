/*
 * A340335  E.g.f.: Sum_{n>=0} x^n * exp(2^n*x).
 * https://oeis.org/A340335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} 2^{(n-k)k} / k! (OEIS A340335)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    seq_t term = factorial(n);          /* term_0 = n! */
    for (idx_t j = 0; j <= n; ++j) {
        sum += term;
        if (j == n)            /* last term added, exit */
            break;

        /* Compute exponent for the next term: exp = n - 2*j - 1 */
        long long exp = (long long)n - 2LL * (long long)j - 1LL;

        if (exp >= 0) {
            term = (term * pow2((idx_t)exp)) / (j + 1);
        } else {
            term = term / pow2((idx_t)(-exp));
            term = term / (j + 1);
        }
    }
    return sum;
}
