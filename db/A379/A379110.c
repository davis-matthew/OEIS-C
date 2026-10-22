/*
 * A379110  Dirichlet inverse of A324892, where A324892 is multiplicative with
 *          a(p^e) = p^e if sigma(p^e) is prime, and 1 otherwise.
 * https://oeis.org/A379110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the Dirichlet inverse of the multiplicative function
 *            A324892, i.e. a(1)=1 and for n>1 a(n) = -∑_{d|n, d<n}
 *            A324892(n/d) * a(d). For multiplicativity this reduces to a(p^e)
 *            = -∑_{k=1..e} A324892(p^k) * a(p^{e-k}) (p prime, e≥1), with
 *            A324892(p^k) = p^k if σ(p^k) is prime else 1. The value...
 * Range:     i8 n <= 16, i16 n <= 8191, i32 n <= 9840, i64 n <= 9840,
 *            i128 n <= 9840, big any n
 * Helpers:   factor_u64, is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the helper function primes() is not required because
   factor_u64 works with its own prime table. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"        /* factor_u64 */
#include "../utils/is_prime_big.h"      /* is_prime_big */
#include "../utils/seq_pow.h"           /* seq_pow */

static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    /* Auxiliary arrays: maximal exponent for p^e <= 10000 is < 20 */
    static const size_t MAX_EXP = 20;
    seq_t f[MAX_EXP];
    seq_t a[MAX_EXP];
    if (e >= MAX_EXP) {    /* safety for extremely large e (never happens here) */
        /* fall back to iterative computation with dynamic allocation */
        seq_t *f_dyn = (seq_t*)malloc((e+1)*sizeof(seq_t));
        seq_t *a_dyn = (seq_t*)malloc((e+1)*sizeof(seq_t));
        seq_t ppow = 1;
        f_dyn[0] = 1;
        a_dyn[0] = 1;
        for (uint64_t k=1; k<=e; ++k) {
            ppow *= (seq_t)p;
            seq_t pow_plus = seq_pow((seq_t)p, k+1);
            seq_t sigma = (pow_plus - (seq_t)1) / ((seq_t)(p-1));
            f_dyn[k] = is_prime_big(sigma) ? ppow : (seq_t)1;
        }
        for (uint64_t k=1; k<=e; ++k) {
            seq_t sum = 0;
            for (uint64_t j=1; j<=k; ++j)
                sum += f_dyn[j] * a_dyn[k-j];
            a_dyn[k] = -sum;
        }
        seq_t result = a_dyn[e];
        free(f_dyn);
        free(a_dyn);
        return result;
    }

    seq_t ppow = 1;
    f[0] = 1;
    a[0] = 1;
    for (uint64_t k=1; k<=e; ++k) {
        ppow *= (seq_t)p;                      /* p^k */
        seq_t pow_plus = seq_pow((seq_t)p, k+1); /* p^(k+1) */
        seq_t sigma = (pow_plus - (seq_t)1) / ((seq_t)(p-1));
        f[k] = is_prime_big(sigma) ? ppow : (seq_t)1;
    }
    for (uint64_t k=1; k<=e; ++k) {
        seq_t sum = 0;
        for (uint64_t j=1; j<=k; ++j)
            sum += f[j] * a[k-j];
        a[k] = -sum;
    }
    return a[e];
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt;
    /* factor_u64 returns the prime factors and exponents of nn */
    factor_u64(nn, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t val = a_prime_power(pr[i], ex[i]);
        result *= val;
    }
    return result;
}
