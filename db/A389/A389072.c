/*
 * A389072  Möbius transform of A325974, where A325974 is the arithmetic mean
 *          of {sum of non-unitary divisors} and {sum of nonsquarefree
 *          divisors}.
 * https://oeis.org/A389072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(d) * A325974(n/d)
 * Range:     u8 n <= 71, u16 n <= 9921, u32 n <= 9921, u64 n <= 9921,
 *            u128 n <= 9921, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n - A388286(n) */
/* a(n) = n * (1 - mu(n)^2/2) - A254503(n)/2  for n >= 1 */

#include <stdint.h>
#include <stdlib.h>

/* static state for memoised precomputation */
static seq_t *A_arr = NULL;          /* A325974 values */
static int8_t *mu_arr = NULL;       /* Möbius μ values (signed) */
static uint64_t *spf_arr = NULL;    /* smallest prime factor */
static size_t g_max_precomp = 0;    /* max n we have precomputed */

/* Ensure all values up to N are precomputed */
static void precompute(idx_t N)
{
    if (N <= g_max_precomp)
        return;

    /* grow arrays to size N+1 */
    seq_t *newA = (seq_t *)calloc((size_t)N + 1, sizeof(seq_t));
    int8_t *newMu = (int8_t *)calloc((size_t)N + 1, sizeof(int8_t));
    uint64_t *newSpf = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));

    /* linear sieve for spf and μ */
    uint64_t *primes = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    size_t pcnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!newSpf[i]) {
            newSpf[i] = i;
            newMu[i] = -1;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint64_t p = primes[j];
            uint64_t t = i * p;
            if (t > N)
                break;
            newSpf[t] = p;
            if (i % p == 0) {
                newMu[t] = 0;
                break;
            } else {
                newMu[t] = -newMu[i];
            }
        }
    }
    newMu[1] = 1;   /* μ(1) = 1 */

    /* compute A325974 for each n */
    newA[1] = 0;
    for (idx_t n = 2; n <= N; ++n) {
        seq_t sigma = (seq_t)1;          /* σ(n) */
        seq_t sigma_u = (seq_t)1;        /* sum of unitary divisors */
        seq_t sigma_sqfree = (seq_t)1;   /* sum of square‑free divisors */
        uint64_t m = n;
        while (m > 1) {
            uint64_t p = newSpf[m];
            uint64_t power = 1;
            seq_t sum_powers = (seq_t)1; /* 1 + p + … + p^e */
            while (m % p == 0) {
                m /= p;
                power *= p;
                sum_powers += (seq_t)power;
            }
            sigma      *= sum_powers;
            sigma_u    *= (seq_t)(1 + power);
            sigma_sqfree*= (seq_t)(1 + p);
        }
        seq_t tmp = sigma * 2 - sigma_u - sigma_sqfree;
        newA[n] = tmp / 2;   /* integer division safe */
    }

    free(primes);

    /* release old buffers */
    free(A_arr);
    free(mu_arr);
    free(spf_arr);

    /* install new ones */
    A_arr = newA;
    mu_arr = newMu;
    spf_arr = newSpf;
    g_max_precomp = (size_t)N;
}

/* ------------------------------------------------------------------ */
/*   required sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* safety for 0 */

    precompute(n);

    long long total = 0;  /* signed to hold possible negative parts */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;
        idx_t d2 = n / d;
        int8_t mu_d = mu_arr[d];
        if (mu_d != 0)
            total += (long long)mu_d * (long long)A_arr[d2];
        if (d2 != d) {
            int8_t mu_d2 = mu_arr[d2];
            if (mu_d2 != 0)
                total += (long long)mu_d2 * (long long)A_arr[d];
        }
    }
    return (seq_t)total;
}
