/*
 * A285854  Number of permutations of [n] with three ordered cycles such that
 *          equal-sized cycles are ordered with increasing least elements.
 * https://oeis.org/A285854
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of [n] with three ordered cycles *
 *            where equal sized cycles are ordered with increasing least
 *            elements. * Computation is performed by summing over all
 *            unordered triples of * positive integers a ≤ b ≤ c with a + b +
 *            c = n. For each triple the * contribution is * * n!...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 3) the value is undefined.
     * Return 0 to satisfy external linkage. */
    if (n < 3) return (seq_t)0;

    /* n! as a big integer */
    seq_t fact_n = factorial(n);

    seq_t ans = (seq_t)0;

    for (idx_t a_val = 1; a_val <= n - 2; ++a_val) {
        for (idx_t b_val = a_val; b_val <= n - a_val - 1; ++b_val) {
            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val) continue;   /* ensure a ≤ b ≤ c */

            /* Determine multiplicity factor mfac and weight w */
            unsigned mfac = 1;
            unsigned w     = 6;   /* all distinct by default */

            if (a_val == b_val && b_val == c_val) {   /* all equal */
                mfac = 6;
                w     = 1;
            } else if (a_val == b_val || b_val == c_val) { /* two equal */
                mfac = 2;
                w     = 3;
            }

            /* Denominator = a * b * c * mfac   (fits in 64 or 128 bits) */
            seq_t denom = (seq_t)a_val * (seq_t)b_val * (seq_t)c_val;
            denom = denom * (seq_t)mfac;

            /* term = (fact_n / denom) * w */
            seq_t term = fact_n / denom;
            term = term * (seq_t)w;

            ans = ans + term;
        }
    }

    return ans;
}
