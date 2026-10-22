/*
 * A338714  Number of numbers of the form i*n with 1 <= i <= n and tau(i*n) =
 *          6.
 * https://oeis.org/A338714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ i : 1 <= i <= n, tau(i*n) = 6 }
 * Range:     u8 n <= 1680, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a straightforward factor‑based counting with a dynamic SPF sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache */
    static idx_t max_limit = 0;
    static uint64_t *spf = NULL;          /* smallest prime factor for each integer */
    static uint32_t *prime_index = NULL;  /* map prime -> index in primes array (or NO_PRIME_INDEX) */
    static uint32_t *primes = NULL;       /* list of primes in ascending order */
    static size_t prime_count = 0;        /* number of primes up to max_limit */
    static const uint32_t NO_PRIME_INDEX = 0xFFFFFFFFu;

    /* enlarge cache if necessary */
    if (n > max_limit) {
        idx_t new_limit = n;
        /* allocate new spf array */
        uint64_t *new_spf = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
        if (!new_spf) return (seq_t)0;
        memset(new_spf, 0, (new_limit + 1) * sizeof(uint64_t));

        /* compute spf with a simple sieve */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (new_spf[i] == 0) {
                new_spf[i] = i;
                if (i * i <= new_limit)
                    for (idx_t j = i * i; j <= new_limit; j += i)
                        if (new_spf[j] == 0) new_spf[j] = i;
            }
        }

        /* collect primes and build prime_index table */
        size_t pcount = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (new_spf[i] == i) ++pcount;

        uint32_t *new_primes = (uint32_t*)malloc(pcount * sizeof(uint32_t));
        uint32_t *new_prime_index = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
        if (!new_primes || !new_prime_index) {
            free(new_spf); free(new_primes); free(new_prime_index);
            return (seq_t)0;
        }
        memset(new_prime_index, 0xFF, (new_limit + 1) * sizeof(uint32_t));

        size_t idx = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (new_spf[i] == i) {
                new_primes[idx] = (uint32_t)i;
                new_prime_index[i] = (uint32_t)idx;
                ++idx;
            }

        /* free old caches */
        free(spf);
        free(prime_index);
        free(primes);

        /* install new caches */
        spf = new_spf;
        prime_index = new_prime_index;
        primes = new_primes;
        prime_count = pcount;
        max_limit = new_limit;
    }

    /* factorise n and store exponents in exp_n array */
    uint8_t *exp_n = (uint8_t*)calloc(prime_count, sizeof(uint8_t));
    if (!exp_n) return (seq_t)0;
    idx_t t = n;
    while (t > 1) {
        uint64_t p = spf[t];
        uint8_t e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        uint32_t idx = prime_index[p];
        if (idx != NO_PRIME_INDEX)
            exp_n[idx] = e;
    }

    /* compute tau(n) */
    seq_t tau_n = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint8_t e = exp_n[i];
        if (e) tau_n *= (seq_t)(e + 1);
    }

    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t tau_val = tau_n;           /* start from tau(n) */
        uint64_t x = i;
        while (x > 1) {
            uint64_t p = spf[x];
            uint8_t e_i = 0;
            while (x % p == 0) {
                x /= p;
                ++e_i;
            }
            uint32_t idxp = prime_index[p];
            if (idxp != NO_PRIME_INDEX) {
                uint8_t e_n = exp_n[idxp];
                seq_t oldf = (seq_t)(e_n + 1);
                seq_t newf = (seq_t)(e_n + e_i + 1);
                tau_val = tau_val / oldf * newf;
            } else {
                tau_val *= (seq_t)(e_i + 1);
            }
        }
        if (tau_val == (seq_t)6) ++result;
    }

    free(exp_n);
    return result;
}
