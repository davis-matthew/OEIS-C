/*
 * A297527  Number of maximum matchings in the complete tripartite graph
 *          K_{n,n,n}.
 * https://oeis.org/A297527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2))^3 * floor(n/2)! * ceil(n/2)!^2 *
 *            (2-(-1)^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 95 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset, return 0 */
    idx_t k = n / 2;                      /* floor(n/2)   */
    idx_t m = n - k;                      /* ceil(n/2)    */

    seq_t bin   = binomial(n, k);          /* C(n,k)       */
    seq_t fac_k = factorial(k);           /* k!           */
    seq_t fac_m = factorial(m);           /* m!           */

    seq_t fac_m2 = fac_m;
    fac_m2 *= fac_m;                      /* (m!)^2       */

    seq_t res = bin;
    res *= bin;
    res *= bin;                            /* bin^3        */
    res *= fac_k;
    res *= fac_m2;                         /* *m!^2        */

    if ((n & (idx_t)1) == (idx_t)1) {      /* odd n → factor 3 */
        res *= (seq_t)3;
    }

    return res;
}
