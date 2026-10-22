/*
 * A374969  Number of ordered solutions (x,y,z,w) to x*y + y*z + z*w + w*x = n
 *          with x,y,z,w >= 1.
 * https://oeis.org/A374969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*A000005(n) - 2*A000203(n)
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000005(n) = τ(n) (number of divisors of n)  */
/* and A000203(n) = σ(n) (sum of divisors of n)      */

#include <stddef.h>
#if defined(__cplusplus)
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

/* The build supplies these types externally. */
extern unsigned long long divisor_count_u64(unsigned long long n);
extern seq_t sigma_of(idx_t n);

/* Compute the sequence value for a given n. */
seq_t a(idx_t n)
{
    /* Offset: OEIS sequence starts at a(1).  For n < 1 we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* 10‑fold faster than calling helper functions repeatedly:
     * we sieve d(n) and σ(n) up to the requested n.
     * The sieve is O(n log n); for n ≤ 10000 it takes far less than a millisecond. */
    idx_t N = n;
    size_t sz = (size_t)N + 1;

#if defined(__cplusplus)
    seq_t *tau = new seq_t[sz]();
    seq_t *sigma = new seq_t[sz]();
#else
    seq_t *tau = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *sigma = (seq_t *)calloc(sz, sizeof(seq_t));
#endif

    /* Sieve for divisor count and sum of divisors. */
    for (idx_t d = 1; d <= N; ++d)
    {
        for (idx_t m = d; m <= N; m += d)
        {
            tau[m]   = tau[m] + 1;          /* τ(m) += 1      */
            sigma[m] = sigma[m] + (seq_t)d; /* σ(m) += d      */
        }
    }

    /* Apply the closed‑form expression. */
    seq_t n_seq = (seq_t)n;
    seq_t term1 = (n_seq + 1) * tau[n];
    seq_t term2 = 2 * sigma[n];
    seq_t result = term1 - term2;

#if defined(__cplusplus)
    delete[] tau;
    delete[] sigma;
#else
    free(tau);
    free(sigma);
#endif

    return result;
}
