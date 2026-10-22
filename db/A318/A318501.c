/*
 * A318501  a(n) = SumXOR_{d|n} [moebius(n/d) > 0]*(sigma(d)-d).
 * https://oeis.org/A318501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SumXOR_{d|n} [moebius(n/d) > 0]*(sigma(d)-d)
 * Range:     u8 n <= 143, u16 n <= 9463, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the XOR of (σ(d)−d) over all divisors d of n
   for which μ(n/d)=1.  The sequence is defined for n≥1. */

/* Standard includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Static cache structures for σ, μ and smallest prime factors (spf) */
/* ------------------------------------------------------------------ */
static seq_t *sigma_cache = NULL;      /* sigma[0..max]          */
static int8_t *mu_cache   = NULL;      /* μ[0..max]  (-1,0,1)   */
static uint32_t *spf_cache = NULL;     /* smallest prime factor  */
static size_t built_max = 0;            /* currently built range   */

/* ------------------------------------------------------------------ */
/* Build tables up to at least max_n.                                 */
/* ------------------------------------------------------------------ */
static void build_upto(idx_t max_n)
{
    if ((idx_t)built_max >= max_n)
        return;

    /* Allocate new tables (zero‑initialised) */
    size_t new_size   = (size_t)(max_n + 1);
    seq_t *new_sigma  = (seq_t *)calloc(new_size, sizeof(seq_t));
    int8_t *new_mu    = (int8_t *)calloc(new_size, sizeof(int8_t));
    uint32_t *new_spf = (uint32_t *)calloc(new_size, sizeof(uint32_t));
    if (!new_sigma || !new_mu || !new_spf) {
        abort();                       /* Allocation failure */
    }

    /* Linear sieve to fill spf and mobius μ */
    uint32_t *primes = NULL;
    size_t primes_cnt = 0, primes_cap = 0;

    for (idx_t i = 2; i <= max_n; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes,
                                              primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t prod = i * (idx_t)p;
            if (prod > max_n)
                break;
            new_spf[prod] = p;
            if (i % p == 0) {
                new_mu[prod] = 0;
                break;
            } else {
                new_mu[prod] = -new_mu[i];
            }
        }
    }
    new_mu[1] = 1;   /* μ(1) = 1 */

    /* Compute divisor sums σ[n] = Σ_{d|n} d */
    for (idx_t i = 1; i <= max_n; ++i) {
        for (idx_t j = i; j <= max_n; j += i) {
            new_sigma[(size_t)j] += (seq_t)i;
        }
    }

    /* Free old tables and cache the new ones */
    free(sigma_cache);
    free(mu_cache);
    free(spf_cache);
    free(primes);

    sigma_cache = new_sigma;
    mu_cache    = new_mu;
    spf_cache   = new_spf;
    built_max   = (size_t)max_n;
}

/* ------------------------------------------------------------------ */
/* Generate all divisors of n.                                         */
/* ------------------------------------------------------------------ */
static void get_divisors(idx_t n, idx_t *array, size_t *len)
{
    array[0] = 1;
    size_t cnt = 1;

    idx_t temp = n;
    while (temp > 1) {
        idx_t p = (idx_t)spf_cache[(size_t)temp];
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        size_t cur_cnt = cnt;
        idx_t p_pow = 1;
        for (idx_t e = 1; e <= exp; ++e) {
            p_pow *= p;
            for (size_t i = 0; i < cur_cnt; ++i) {
                array[cnt++] = array[i] * p_pow;
            }
        }
    }

    *len = cnt;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, but safe guard */

    build_upto(n);

    /* Collect divisors – up to 256 suffice for n ≤ 10⁴ */
    static idx_t divisors[256];
    size_t dcnt = 0;
    get_divisors(n, divisors, &dcnt);

    seq_t result = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d  = divisors[i];
        idx_t q  = n / d;                 /* n/d */
        if (mu_cache[(size_t)q] == 1) {    /* μ(q) = 1 */
            seq_t val = sigma_cache[(size_t)d] - (seq_t)d;
            result ^= val;
        }
    }
    return result;
}
