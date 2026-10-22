/*
 * A373041  2*a(n) is the number of triangles with integer sides (x, y, n), x
 *          < y < n, and gcd(x, y, n) = 1.
 * https://oeis.org/A373041
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ ∑_{d|n} μ(d) ⋅ S(n/d) , where S(m) = (m/2−1)² for even
 *            m, ((m−1)/2)⋅((m−3)/2) for odd m (S(m) is the number of integer
 *            pairs 1≤x<y<m with x+y>m)
 * Range:     u8 n <= 33, u16 n <= 725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _S_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_S_size_t.h"

/* global tables for Möbius function and smallest prime factors */
static size_t _mu_max = 0;
static int8_t  * _mu = NULL;    /* μ[0…_mu_max]   */
static uint32_t * _spf = NULL;  /* smallest prime factor */

/* Build μ and spf up to N (inclusive) using a linear sieve */
static void _ensure_mu_spf(size_t N)
{
    if (N <= _mu_max) return;

    /* Allocate fresh arrays and zero them */
    int8_t  *mu_new  = (int8_t  *)calloc(N + 1, sizeof(int8_t));
    uint32_t *spf_new = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!mu_new || !spf_new) { free(mu_new); free(spf_new); exit(EXIT_FAILURE); }

    /* Linear sieve */
    size_t *primes = (size_t *)malloc((N + 1) * sizeof(size_t));
    size_t pc = 0;
    mu_new[1] = 1; /* μ(1) = 1 */

    for (size_t i = 2; i <= N; ++i) {
        if (!spf_new[i]) {                /* i is prime */
            spf_new[i] = (uint32_t)i;
            primes[pc++] = i;
            mu_new[i] = -1;               /* μ(p) = -1 */
        }
        for (size_t j = 0; j < pc; ++j) {
            size_t p = primes[j];
            size_t ip = i * p;
            if (ip > N) break;
            if (p > spf_new[i]) break;    /* enforce i%p==0 condition */
            spf_new[ip] = (uint32_t)p;
            if (i % p == 0) {
                mu_new[ip] = 0;           /* square factor */
                break;
            } else {
                mu_new[ip] = -mu_new[i];
            }
        }
    }
    free(primes);

    /* Free old tables and adopt new ones */
    free(_mu);
    free(_spf);
    _mu   = mu_new;
    _spf  = spf_new;
    _mu_max = N;
}

/* --------------------------------------------------------------- */
/*  externally linked function requested by the specification          */
seq_t a(idx_t n)
{
    /* behaviour for n < 5 is irrelevant */
    if (n < 5u) return 0;

    size_t N = (size_t)n;
    _ensure_mu_spf(N);

    int64_t sum = 0;

    /* Enumerate all divisors d of N */
    for (size_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            size_t d1 = i;
            size_t d2 = N / i;

            /* first divisor */
            int8_t mu_d1 = _mu[d1];
            if (mu_d1 != 0) {
                size_t m1 = N / d1;          /* m = N/d1 */
                seq_t s1 = _S_size_t(m1);
                sum += (int64_t)mu_d1 * (int64_t)s1;
            }

            /* second divisor, if distinct */
            if (d2 != d1) {
                int8_t mu_d2 = _mu[d2];
                if (mu_d2 != 0) {
                    size_t m2 = N / d2;
                    seq_t s2 = _S_size_t(m2);
                    sum += (int64_t)mu_d2 * (int64_t)s2;
                }
            }
        }
    }

    /* a(n) = sum / 2  (sum is guaranteed to be even for n ≥ 5) */
    return (seq_t)(sum / 2);
}
