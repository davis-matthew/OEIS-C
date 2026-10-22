/*
 * A394593  Sum of the distinct prime divisors of C(7*n, n).
 * https://oeis.org/A394593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct prime divisors of C(7*n,n)
 * Range:     u8 n <= 10, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute each prime p <= 7*n, test its exponent in   */
/* the binomial coefficient by Legendre's formula, and sum those p whose */
/* exponent is > 0.  Works for any seq_t/idx_t types supported by the  */
/* build.                                                            */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"          /* build_primes(idx_t, idx_t **, size_t *) */

static inline idx_t vp_factorial(idx_t k, uint64_t p) {
    idx_t e = 0;
    for (k /= p; k; k /= p) {
        e += k;
    }
    return e;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* defined for safety, not used by OEIS */
    idx_t limit = 7 * n;                 /* upper bound for primes to check */

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &primes, &prime_cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        idx_t exp = vp_factorial(limit, p) - vp_factorial(n, p)
                      - vp_factorial(limit - n, p);   /* exponent in C(7n,n) */
        if (exp > 0) {
            sum += (seq_t)p;               /* distinct prime contributes once */
        }
    }

    free(primes);
    return sum;
}
