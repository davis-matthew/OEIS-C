/*
 * A331771  a(n) = Sum_{-n<i<n, -n<j<n, gcd{i,j}=1} (n-|i|)*(n-|j|).
 * https://oeis.org/A331771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * ((n-1)*(2n-1) + Σ_{i=2}^{n-1} (n-i)*(2n-i)*phi(i))
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A331771) */
/* The implementation uses a simple sieve for Euler’s totient function.
   It is compatible with both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Cache for phi values */
#ifdef __cplusplus
static std::vector<seq_t> phi_cache_vec;
static idx_t phi_cache_max = 0;
#else
static seq_t *phi_cache = NULL;
static idx_t phi_cache_max = 0;
#endif

/* Access macro for the cached phi value */
#ifdef __cplusplus
#  define GET_PHI(i) phi_cache_vec[(i)]
#else
#  define GET_PHI(i) phi_cache[(i)]
#endif

/* Ensure that phi_cache has values for 1..upto (inclusive) */
#ifdef __cplusplus
static void ensure_phi(idx_t upto)
{
    if (upto <= phi_cache_max)
        return;
    phi_cache_vec.resize(upto + 1);
    for (idx_t i = 0; i <= upto; ++i)
        phi_cache_vec[i] = (seq_t)i;          /* phi(i) = i initially */

    for (idx_t i = 2; i <= upto; ++i) {
        if (phi_cache_vec[i] == (seq_t)i) {   /* i is prime */
            for (idx_t j = i; j <= upto; j += i) {
                phi_cache_vec[j] -= phi_cache_vec[j] / (seq_t)i;
            }
        }
    }
    phi_cache_max = upto;
}
#else
static void ensure_phi(idx_t upto)
{
    if (upto <= phi_cache_max)
        return;
    if (phi_cache)
        free(phi_cache);
    phi_cache = malloc((upto + 1) * sizeof(seq_t));
    if (!phi_cache) {
        exit(EXIT_FAILURE);
    }
    for (idx_t i = 0; i <= upto; ++i)
        phi_cache[i] = (seq_t)i;                /* phi(i) = i initially */

    for (idx_t i = 2; i <= upto; ++i) {
        if (phi_cache[i] == (seq_t)i) {         /* i is prime */
            for (idx_t j = i; j <= upto; j += i) {
                phi_cache[j] -= phi_cache[j] / (seq_t)i;
            }
        }
    }
    phi_cache_max = upto;
}
#endif

seq_t a(idx_t n)
{
    if (n <= 1U)          /* offset 1: a(1) = 0 */
        return 0;

    ensure_phi(n - 1U);

    seq_t sum = 0;
    seq_t n_t  = (seq_t)n;

    /* term for i=1 is (n-1)*(2n-1) */
    seq_t term0 = (n_t - 1) * ((seq_t)2 * n_t - 1);
    sum += term0;

    for (idx_t i = 2; i <= n - 1U; ++i) {
        seq_t i_t     = (seq_t)i;
        seq_t ni      = n_t - i_t;
        seq_t twoN_i  = (seq_t)2 * n_t - i_t;
        sum += ni * twoN_i * GET_PHI(i);
    }

    sum *= 4;
    return sum;
}
