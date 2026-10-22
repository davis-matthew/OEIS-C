/*
 * A342415  a(n) = phi(n) / gcd(phi(n),A003415(n)), where A003415(n) is the
 *          arithmetic derivative of n, and phi is Euler totient function.
 * https://oeis.org/A342415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) / gcd(phi(n), A003415(n))
 * Range:     u8 n <= 256, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   count_distinct_primes, factor_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003415(n) is the arithmetic derivative of n             */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/phi.h"

/* Compute the arithmetic derivative of n */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1) return 0;          /* derivative of 0 or 1 is defined as 0 */

    /* Determine number of distinct prime factors to size allocation */
    size_t cnt = count_distinct_primes(n);
    if (cnt == 0) return 0;       /* for n=1, but already handled */

    /* Allocate arrays for primes and exponents */
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    uint64_t *ex = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t actual_cnt = 0;

    /* Perform factorisation: fill pr, ex, and actual_cnt */
    factor_u64(n, pr, ex, &actual_cnt);

    seq_t nseq = (seq_t)n;
    seq_t result = 0;

    for (size_t i = 0; i < actual_cnt; ++i) {
        seq_t term = (nseq / (seq_t)pr[i]) * (seq_t)ex[i];
        result += term;
    }

    free(pr);
    free(ex);
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined; return 0 as a fallback */

    seq_t phi_val = phi(n);
    seq_t deriv = arithmetic_derivative(n);
    seq_t g = gcd(phi_val, deriv);
    return phi_val / g;
}
