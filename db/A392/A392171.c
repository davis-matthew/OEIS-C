/*
 * A392171  The Dedekind psi function value of the smallest cubefull number
 *          that is a multiple of n.
 * https://oeis.org/A392171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} (p+1)·p^{max(e,3)-1} (multiplicative
 *            definition)
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1229, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but handle gracefully */
        return 0;

    seq_t result = 1;
    idx_t nn = n;

    /* factor n by trial division */
    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            idx_t exp = 0;
            while (nn % p == 0) {
                ++exp;
                nn /= p;
            }
            idx_t e_needed = (exp < 3 ? 3 : exp);
            seq_t pow_p = seq_pow((seq_t)p, e_needed - 1);
            seq_t term  = (seq_t)(p + 1) * pow_p;
            result *= term;
        }
    }
    /* remaining prime factor, if any */
    if (nn > 1) {
        idx_t exp = 1;
        idx_t e_needed = 3; /* since exp < 3 */
        seq_t pow_p = seq_pow((seq_t)nn, e_needed - 1);  /* exponent 2 */
        seq_t term  = (seq_t)(nn + 1) * pow_p;
        result *= term;
    }

    return result;
}
