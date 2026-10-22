/*
 * A326306  Dirichlet g.f.: zeta(s) * zeta(s-1) * Product_{p prime} (1 - p^(1
 *          - s) + p^(-s)).
 * https://oeis.org/A326306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (1 + (p^e − 1)/(p − 1)) – multiplicative
 *            definition (see OEIS A326306)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square‑root: floor(sqrt(n)) – works for uint64_t without overflow. */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {  /* avoid division by zero when n==0 */
            lo = 1;
            continue;
        }
        if (mid <= n / mid)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp   = n;            /* copy of n to factorize */

    for (idx_t p = 2; p <= int_sqrt(temp); ++p) {
        if (temp % p == 0) {
            idx_t exp  = 0;
            idx_t pow  = 1;      /* p^exp, fits in idx_t because it is a divisor of n */

            while (temp % p == 0) {
                temp /= p;
                ++exp;
                pow *= p;        /* safe: pow stays <= n */
            }

            /* term = 1 + (p^exp − 1)/(p − 1) */
            seq_t term = (seq_t)1 + (((seq_t)pow) - (seq_t)1) / (((seq_t)p) - (seq_t)1);
            result *= term;
        }
    }

    if (temp > 1) {              /* remaining prime factor with exponent 1 */
        /* For exp = 1: term = 1 + (p − 1)/(p − 1) = 2 */
        result *= (seq_t)2;
    }

    return result;
}
