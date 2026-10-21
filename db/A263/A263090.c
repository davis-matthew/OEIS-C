/*
 * A263090  Primes p for which A049820(x) = p has a solution.
 * https://oeis.org/A263090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that ∃k: k - d(k) = p (where d(k)
 *            is the divisor count τ(k))
 * Range:     u8 n <= 37, u16 n <= 4246, u32 n <= 7955, u64 n <= 7955,
 *            u128 n <= 7955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Local helper: we use static data to compute the series only once. */
static seq_t *series = NULL;         /* the computed sequence (sorted)          */
static size_t series_len = 0;        /* number of terms in the sequence        */
static bool initialized = false;    /* flag indicating if precomputation was done */

/* ---------------------------------------------------------------------- */
/*  Precomputation: small sieve up to a fixed limit.  The limit is chosen
    large enough to obtain more than 10 000 terms of the sequence.   */
static void precompute(void)
{
    const uint32_t LIMIT = 300000;          /* sieve upper bound */

    /* Allocate arrays needed for the linear sieve.  */
    uint32_t *spf        = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    uint32_t *tau        = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    uint8_t  *exp_arr    = (uint8_t  *)malloc((LIMIT + 1) * sizeof(uint8_t));
    uint8_t  *is_prime   = (uint8_t  *)malloc((LIMIT + 1) * sizeof(uint8_t));
    uint8_t  *prime_seen = (uint8_t  *)malloc((LIMIT + 1) * sizeof(uint8_t)); /* marks p that are in the sequence */
    uint32_t *primes     = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));

    /* Initialize the arrays.  */
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        spf[i]      = 0;
        tau[i]      = 0;
        exp_arr[i]  = 0;
        is_prime[i] = 1;        /* assume prime */
    }
    is_prime[0] = is_prime[1] = 0;
    tau[1] = 1;
    for (uint32_t i = 0; i <= LIMIT; ++i)
        prime_seen[i] = 0;

    /* Linear sieve computing spf, tau and the list of primes.  */
    size_t prime_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                  /* i is prime */
            spf[i]      = i;
            tau[i]      = 2;
            exp_arr[i]  = 1;
            is_prime[i] = 1;
            primes[prime_cnt++] = i;
        }

        for (size_t j = 0; j < prime_cnt &&
                           primes[j] <= spf[i] &&
                           (uint64_t)i * primes[j] <= LIMIT; ++j) {
            uint32_t v = i * primes[j];
            spf[v] = primes[j];
            is_prime[v] = 0;                 /* composite */

            if (primes[j] == spf[i]) {
                /* same prime as i's spf: extend its exponent */
                exp_arr[v] = exp_arr[i] + 1;
                tau[v] = tau[i] / (exp_arr[i] + 1) * (exp_arr[v] + 1);
            } else {
                /* new prime factor */
                exp_arr[v] = 1;
                tau[v] = tau[i] * 2;
            }
        }
    }

    /* Find all primes p for which some k satisfies k - τ(k) = p. */
    for (uint32_t k = 2; k <= LIMIT; ++k) {
        uint32_t p = k - tau[k];
        if (p >= 2 && is_prime[p] && !prime_seen[p]) {
            prime_seen[p] = 1;
        }
    }

    /* Count how many such primes we found and allocate the sequence array. */
    series_len = 0;
    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (prime_seen[p]) ++series_len;
    }
    series = (seq_t *)malloc(series_len * sizeof(seq_t));

    /* Now fill the sequence array in increasing order of p. */
    size_t idx = 0;
    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (prime_seen[p]) {
            series[idx++] = (seq_t)p;
        }
    }

    /* Free temporary arrays.  */
    free(spf);
    free(tau);
    free(exp_arr);
    free(is_prime);
    free(prime_seen);
    free(primes);

    initialized = true;
}

/* ---------------------------------------------------------------------- */
/*  Public function required by the problem statement.  */
seq_t a(idx_t n)
{
    if (!initialized)
        precompute();

    if (n == 0 || n > (idx_t)series_len)
        return (seq_t)0;   /* undefined for out-of-range indexes */

    return series[n - 1];
}
