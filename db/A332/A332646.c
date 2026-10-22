/*
 * A332646  Numbers m with a divisor d such that d^tau(d) = m.
 * https://oeis.org/A332646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer m for which there exists a
 *            divisor d of m such that d^τ(d) = m. The values are the numbers
 *            d^τ(d) for all d ≥ 1 sorted in non‑decreasing order. This
 *            implementation precomputes all values up to an index larger than
 *            20 000 and serves arbitrary requests up to that range
 * Range:     u8 none, u16 n <= 3, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   cmp_seq_t, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/cmp_seq_t.h"  /* int cmp_seq_t(const void*, const void*); */

static seq_t *cache = NULL;          /* sorted unique values         */
static size_t cache_len = 0;         /* number of stored values       */
static bool cache_inited = false;    /* initialization flag           */

static void init_sequence(void)
{
    /* Limits chosen to guarantee coverage of the first 20 000 terms
       and to avoid overflow in 64‑bit builds.                         */
    const idx_t MAX_PRIME   = 200000;   /* limit for primes   */
    const idx_t MAX_COMP    = 3000;     /* limit for composites (τ≤4)   */
    const size_t CAPACITY   = 40000;    /* how many values we can store */

    /* Dynamic array for results */
    seq_t *values = (seq_t *)malloc(CAPACITY * sizeof(seq_t));
    size_t values_len = 0;

    /* 1. Add m=1 (d=1). */
    values[values_len++] = (seq_t)1;

    /* 2. Sieve primes up to MAX_PRIME. */
    uint8_t *is_prime = (uint8_t *)malloc((MAX_PRIME + 1) * sizeof(uint8_t));
    for (idx_t i = 0; i <= MAX_PRIME; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes first. */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* Allocate storage for primes and collect them. */
    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) primes[idx++] = i;

    /* 3. Add prime squares: d=p, τ(p)=2, m=p^2. */
    for (size_t k = 0; k < prime_cnt; ++k) {
        seq_t v = (seq_t)primes[k] * (seq_t)primes[k];
        values[values_len++] = v;
    }

    free(primes);   /* primes not needed further */

    /* 4. Compute τ(d) for composites up to MAX_COMP. */
    /* 4a. Smallest prime factors (spf). */
    uint32_t *spf = (uint32_t *)malloc((MAX_COMP + 1) * sizeof(uint32_t));
    for (idx_t i = 2; i <= MAX_COMP; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= MAX_COMP; ++i) {
        if (spf[i] == 0) {         /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= MAX_COMP / i) {
                for (idx_t j = i * i; j <= MAX_COMP; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* 4b. τ(n) via prime‑factorization. */
    uint16_t *tau = (uint16_t *)malloc((MAX_COMP + 1) * sizeof(uint16_t));
    tau[1] = 1;
    for (idx_t n = 2; n <= MAX_COMP; ++n) {
        uint32_t p = spf[n];
        idx_t m = n / p;
        int exp = 1;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        tau[n] = tau[m] * (exp + 1);
    }

    /* 5. Add composite values where τ(d)=3 or 4 (small enough). */
    for (idx_t d = 2; d <= MAX_COMP; ++d) {
        if (tau[d] >= 3 && tau[d] <= 4) {
            seq_t v = pow_uint((uint64_t)d, (idx_t)tau[d]);   /* d^τ(d) */
            values[values_len++] = v;
        }
    }

    free(is_prime);
    free(spf);
    free(tau);

    /* 6. Sort and deduplicate. */
    qsort(values, values_len, sizeof(seq_t), cmp_seq_t);
    size_t uniq_len = 0;
    for (size_t i = 0; i < values_len; ++i) {
        if (i == 0 || values[i] != values[i - 1]) {
            values[uniq_len++] = values[i];
        }
    }
    values_len = uniq_len;

    /* Store results globally. */
    cache = values;
    cache_len = values_len;
}

seq_t a(idx_t n)
{
    if (!cache_inited) {
        init_sequence();
        cache_inited = true;
    }
    if (n == 0 || n > cache_len)
        return (seq_t)0;          /* undefined for out‑of‑range requests */
    return cache[n - 1];
}
