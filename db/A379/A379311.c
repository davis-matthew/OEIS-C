/*
 * A379311  Number of prime indices of n that are 1 or prime.
 * https://oeis.org/A379311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e||n} e * [index(p)=1 ∨ index(p) is prime]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the primes up to a fixed limit (20 000),
   builds the mapping prime → index, and provides fast factorisation for
   n ≤ 10 000. */
/* All required types (seq_t, idx_t) are supplied by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

static const idx_t MAX_LIMIT = 20000;   /* covers all n up to about 10 000 */

static bool initialized = false;
static uint64_t *primes = NULL;            /* list of primes up to MAX_LIMIT */
static size_t   primes_cnt = 0;
static uint16_t *prime_idx_of = NULL;      /* prime_idx_of[p] = index of prime p (1‑based) */
static bool *idx_is_prime = NULL;          /* idx_is_prime[k] = true if k is a prime number */

/* Initialise the prime tables on first use.  */
static void init_primes(void)
{
    if (initialized) return;

    /* Sieve of Eratosthenes up to MAX_LIMIT. */
    bool *is_composite = (bool*)malloc((MAX_LIMIT + 1) * sizeof(bool));
    if (!is_composite) return;
    memset(is_composite, 0, (MAX_LIMIT + 1) * sizeof(bool));

    uint64_t *temp_primes = NULL;
    size_t temp_cap = 0, temp_cnt = 0;

    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (temp_cnt == temp_cap) {
                temp_cap = temp_cap ? temp_cap * 2 : 16;
                uint64_t *newp = (uint64_t*)realloc(temp_primes, temp_cap * sizeof(uint64_t));
                if (!newp) { free(is_composite); free(temp_primes); return; }
                temp_primes = newp;
            }
            temp_primes[temp_cnt++] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    primes = temp_primes;
    primes_cnt = temp_cnt;

    /* Build mapping from prime value to its 1‑based index. */
    prime_idx_of = (uint16_t*)malloc((MAX_LIMIT + 1) * sizeof(uint16_t));
    if (!prime_idx_of) { free(is_composite); free(primes); return; }
    memset(prime_idx_of, 0, (MAX_LIMIT + 1) * sizeof(uint16_t));
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_idx_of[primes[i]] = (uint16_t)(i + 1);   /* indices start at 1 */

    /* Build primality table for indices up to primes_cnt (plus a small margin). */
    size_t max_idx = primes_cnt + 1;
    idx_is_prime = (bool*)malloc((max_idx + 1) * sizeof(bool));
    if (!idx_is_prime) { free(is_composite); free(prime_idx_of); free(primes); return; }
    memset(idx_is_prime, 0, (max_idx + 1) * sizeof(bool));

    bool *is_comp_idx = (bool*)malloc((max_idx + 1) * sizeof(bool));
    memset(is_comp_idx, 0, (max_idx + 1) * sizeof(bool));
    for (idx_t i = 2; i <= max_idx; ++i) {
        if (!is_comp_idx[i]) {
            idx_is_prime[i] = true;
            if ((uint64_t)i * i <= max_idx) {
                for (idx_t j = i * i; j <= max_idx; j += i)
                    is_comp_idx[j] = true;
            }
        }
    }
    free(is_comp_idx);
    free(is_composite);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    init_primes();

    seq_t ans = 0;
    idx_t remaining = n;

    /* Factorise n using the pre‑computed primes. */
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * primes[i] <= remaining; ++i) {
        uint64_t p = primes[i];
        if (remaining % p == 0) {
            size_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            uint16_t idx = prime_idx_of[p];   /* index of prime p (1‑based) */
            if (idx == 1 || idx_is_prime[idx])
                ans += (seq_t)exp;
        }
    }

    /* Handle a remaining prime factor > sqrt(original n). */
    if (remaining > 1) {
        uint64_t p = remaining;
        uint16_t idx = prime_idx_of[p];
        /* In the unlikely event p > MAX_LIMIT, fall back to linear search. */
        if (idx == 0) {
            for (size_t k = 0; k < primes_cnt; ++k) {
                if (primes[k] == p) { idx = (uint16_t)(k + 1); break; }
            }
        }
        if (idx == 1 || idx_is_prime[idx])
            ans += (seq_t)1;
    }

    return ans;
}
