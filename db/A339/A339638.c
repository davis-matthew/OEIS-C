/*
 * A339638  Number of nonempty sets of distinct positive integers that have a
 *          least common multiple <= n.
 * https://oeis.org/A339638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{d|k} μ(k/d) * (2^τ(d) - 1)
 * Range:     u8 n <= 23, u16 n <= 119, u32 n <= 839, u64 n <= 7559,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                 τ = number of divisors (A000005), μ = Möbius function */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* arrays for μ, τ, and Mertens function */
    int *mu = (int *)malloc((size_t)(n + 1) * sizeof(int));
    int *tau = (int *)malloc((size_t)(n + 1) * sizeof(int));
    int *mertens = (int *)malloc((size_t)(n + 1) * sizeof(int));
    unsigned char *isComp =
        (unsigned char *)calloc((size_t)(n + 1), sizeof(unsigned char));

    if (!mu || !tau || !mertens || !isComp) {
        /* allocation failed – return 0 as a safe fallback */
        if (mu) free(mu);
        if (tau) free(tau);
        if (mertens) free(mertens);
        if (isComp) free(isComp);
        return 0;
    }

    /* Linear sieve to compute μ up to n */
    int primes[10000];   /* enough for n <= 10000 */
    int pcnt = 0;
    mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!isComp[i]) {
            primes[pcnt++] = (int)i;
            mu[(size_t)i] = -1;
        }
        for (int j = 0; j < pcnt && (size_t)i * (size_t)primes[j] <= n; ++j) {
            idx_t ip = i * (idx_t)primes[j];
            isComp[(size_t)ip] = 1;
            if (i % primes[j] == 0) {
                mu[(size_t)ip] = 0;
                break;
            } else {
                mu[(size_t)ip] = -mu[(size_t)i];
            }
        }
    }

    /* divisor counts τ(d) */
    for (idx_t i = 0; i <= n; ++i) tau[i] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            tau[(size_t)j] += 1;
        }
    }

    /* Mertens function M(x) = Σ_{m≤x} μ(m) */
    mertens[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        mertens[(size_t)i] = mertens[(size_t)(i - 1)] + mu[(size_t)i];
    }

    /* main summation */
    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int M = mertens[(size_t)(n / d)];
        if (M == 0) continue;
        seq_t term = pow2((idx_t)tau[(size_t)d]) - 1;   /* term = 2^τ(d) - 1 */
        if (M > 0) {
            result += term * (seq_t)M;
        } else /* M < 0 */ {
            result -= term * (seq_t)(-M);
        }
    }

    free(isComp);
    free(mu);
    free(tau);
    free(mertens);
    return result;
}
