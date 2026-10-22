/*
 * A373687  a(n) is the number of ways n can be written as a sum of a
 *          practical number and two pentagonal numbers.
 * https://oeis.org/A373687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4681, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373687
    a(n) = number of ways to write n as P + Q1 + Q2,
    where P is a practical number and Q1,Q2 are pentagonal numbers
    (unordered, allowing Q1=Q2).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

/* Test whether m is a practical number using the list of primes up to m. */
static bool is_practical(uint64_t m, const uint64_t *primes, size_t prime_cnt)
{
    if (m == 1) return true;

    uint64_t temp = m;
    unsigned __int128 sigma = 1; /* product of sigma(p^e) contributions */

    for (size_t i = 0; i < prime_cnt && primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if ((temp % p) != 0) continue;

        unsigned int e = 0;
        while ((temp % p) == 0) {
            temp /= p;
            ++e;
        }

        /* Practicality condition: p <= sigma + 1 */
        if ((uint64_t)(sigma + 1) < p) return false;

        /* Compute σ(p^e) = (p^(e+1)-1)/(p-1) using 128 bit arithmetic. */
        unsigned __int128 pe = 1;
        for (unsigned int t = 0; t < e + 1; ++t) pe *= p;
        unsigned __int128 contrib = (pe - 1) / (p - 1);
        sigma *= contrib;
    }

    if (temp > 1) {
        /* Remaining prime factor with exponent 1. */
        uint64_t p = temp;
        if ((uint64_t)(sigma + 1) < p) return false;
        /* σ(p) = p + 1 */
        sigma *= (p + 1);
    }

    return true;
}

/* Count representations of n as a practical + two pentagonal numbers. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* 1. Generate primes up to n. */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);
    if (!prime_list) return 0;

    /* 2. Build list of pentagonal numbers <= n. */
    uint64_t **pent_ptr = NULL;
    size_t pent_len = 0, pent_cap = 0;
    uint64_t *pent = NULL;
    for (uint64_t k = 0;; ++k) {
        uint64_t v = k * (3 * k - 1) / 2;
        if (v > (uint64_t)n) break;
        if (pent_len == pent_cap) {
            size_t newcap = (pent_cap == 0) ? 16 : (pent_cap << 1);
            uint64_t *tmp = (uint64_t*)realloc(pent, newcap * sizeof(uint64_t));
            if (!tmp) { free(prime_list); return 0; }
            pent = tmp;
            pent_cap = newcap;
        }
        pent[pent_len++] = v;
    }

    /* 3. Compute unordered pair counts of pentagonal sums. */
    unsigned long long *freq = (unsigned long long*)
        calloc((size_t)n + 1, sizeof(unsigned long long));
    if (!freq) { free(prime_list); free(pent); return 0; }

    for (size_t i = 0; i < pent_len; ++i) {
        for (size_t j = i; j < pent_len; ++j) {
            unsigned long long s = pent[i] + pent[j];
            if (s > (unsigned long long)n) break;
            freq[s]++;
        }
    }

    /* 4. Enumerate practical numbers and accumulate counts. */
    unsigned long long total = 0;
    for (uint64_t m = 1; m <= (uint64_t)n; ++m) {
        if (is_practical(m, prime_list, prime_cnt)) {
            uint64_t rem = (uint64_t)n - m;
            if (rem <= (uint64_t)n) total += freq[rem];
        }
    }

    /* 5. Clean up. */
    free(prime_list);
    free(pent);
    free(freq);

    return (seq_t)total;
}
