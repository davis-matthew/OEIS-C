/*
 * A400962  Largest integer e such that, for every k with 1 <= k < n,
 *          binomial(n,k) is divisible by p^e for some prime p (depending on
 *          k).
 * https://oeis.org/A400962
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = min_{1<=k<n} (max_{prime p} v_p(C(n,k)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer e such that for every k (1 ≤ k < n),
   binomial(n,k) is divisible by some prime^e (prime may depend on k). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* not used for n >= 2 */

    /*--- sieve to get primes ≤ n ----------------------------------------*/
    idx_t size = n + 1;
    bool *isPrime = (bool *)malloc(size);
    for (idx_t i = 0; i < size; ++i) isPrime[i] = true;
    if (size > 0) isPrime[0] = false;
    if (size > 1) isPrime[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= n; j += i) isPrime[j] = false;
        }
    }
    size_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i) if (isPrime[i]) ++prime_count;
    idx_t *primes = (idx_t *)malloc(prime_count * sizeof(idx_t));
    size_t pc = 0;
    for (idx_t i = 2; i <= n; ++i) if (isPrime[i]) primes[pc++] = i;
    free(isPrime);

    /*--- allocate arrays ----------------------------------------------*/
    uint32_t *max_of_k = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint32_t *vp_fact  = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));

    /*--- for each prime, compute v_p(n!), then for all k ----------------*/
    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = primes[pi];
        vp_fact[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            idx_t temp = i;
            uint32_t cnt = 0;
            while (temp % p == 0) {
                ++cnt;
                temp /= p;
            }
            vp_fact[i] = vp_fact[i - 1] + cnt;
        }
        uint32_t vp_n = vp_fact[n];
        for (idx_t k = 1; k < n; ++k) {
            uint32_t val = vp_n - vp_fact[k] - vp_fact[n - k];
            if (val > max_of_k[k]) max_of_k[k] = val;
        }
    }

    /*--- find the minimal maximal valuation ----------------------------*/
    uint32_t best = UINT32_MAX;
    for (idx_t k = 1; k < n; ++k) {
        if (max_of_k[k] < best) best = max_of_k[k];
    }

    /*--- clean up ------------------------------------------------------*/
    free(primes);
    free(vp_fact);
    free(max_of_k);

    return (seq_t)best;
}
