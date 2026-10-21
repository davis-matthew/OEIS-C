/*
 * A295268  Expansion of e.g.f. 2/(1 + sqrt(1 - 4*LambertW(x))).
 * https://oeis.org/A295268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=0}^{n-1} (-1)^m * (n-1)!/m! * n^m * binom(2n-2m,
 *            n-1-m). a(0) = 1. See OEIS A295268
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   binom, factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"
#include "../utils/factorial.h"
#include "../utils/binom.h"

/* Return the n-th term of sequence A295268. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */

    /* (n-1)!  needed for all terms */
    seq_t fact_n_minus1 = factorial(n - 1);

    seq_t result = 0;
    seq_t pow_n = 1;         /* n^m, m starts at 0 */
    seq_t fact_m = 1;        /* m!   , m starts at 0 */

    for (idx_t m = 0; m < n; ++m) {
        /* Q = (n-1)! / m!   is an exact integer */
        seq_t Q = fact_n_minus1 / fact_m;

        /* binomial coefficient  C(2n-2m , n-1-m) */
        idx_t top    = 2 * n - 2 * m;
        idx_t bottom = n - 1 - m;
        seq_t binom_part = binom(top, bottom);

        /* term = n^m * binom_part * Q  */
        seq_t term = pow_n * binom_part * Q;

        /* add or subtract according to (-1)^m */
        if (m & 1u)
            result -= term;
        else
            result += term;

        /* prepare for next m */
        pow_n *= (seq_t)n;          /* n^(m+1)  */
        fact_m *= (seq_t)(m + 1);   /* (m+1)!   */
    }

    return result;
}
