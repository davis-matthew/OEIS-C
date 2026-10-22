/*
 * A370504  The smallest positive number such that sopfr(|a(n) - n|) =
 *          sopfr(a(n) + n) and Omega(|a(n) - n|) = Omega(a(n) + n), where
 *          sopfr(k) is the sum of the primes dividing k, with repetition.
 * https://oeis.org/A370504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the minimal m>0 such that
   sopfr(|m-n|) = sopfr(m+n)  and
   Omega(|m-n|) = Omega(m+n).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const idx_t LIMIT_SIEVE = (idx_t)1000000;

static seq_t *sopfr_arr = NULL;
static seq_t *omega_arr = NULL;
static uint32_t *spf_arr = NULL;
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static bool initialized = false;

/* Build a sieve of smallest prime factor up to LIMIT_SIEVE
   and precompute sopfr and omega for all numbers <= LIMIT_SIEVE. */
static void init(void)
{
    if (initialized) return;
    /* allocate arrays */
    spf_arr = (uint32_t *)malloc((LIMIT_SIEVE + 1) * sizeof(uint32_t));
    sopfr_arr = (seq_t *)malloc((LIMIT_SIEVE + 1) * sizeof(seq_t));
    omega_arr = (seq_t *)malloc((LIMIT_SIEVE + 1) * sizeof(seq_t));
    if (!spf_arr || !sopfr_arr || !omega_arr) return; /* allocation failure */

    /* sieve for smallest prime factor */
    for (idx_t i = 0; i <= LIMIT_SIEVE; ++i) spf_arr[i] = 0;
    sopfr_arr[0] = sopfr_arr[1] = 0;
    omega_arr[0] = omega_arr[1] = 0;

    uint32_t *tmp_primes = (uint32_t *)malloc(LIMIT_SIEVE * sizeof(uint32_t));
    size_t pl = 0;
    for (idx_t i = 2; i <= LIMIT_SIEVE; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (uint32_t)i;
            tmp_primes[pl++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pl && (uint64_t)tmp_primes[j] * i <= LIMIT_SIEVE; ++j) {
            uint32_t p = tmp_primes[j];
            spf_arr[i * p] = p;
            if (i % p == 0) break;
        }
    }
    prime_count = pl;
    primes = tmp_primes;

    /* precompute sopfr and omega */
    for (idx_t i = 2; i <= LIMIT_SIEVE; ++i) {
        uint32_t p = spf_arr[i];
        idx_t rem = i / p;
        sopfr_arr[i] = sopfr_arr[rem] + (seq_t)p;
        omega_arr[i] = omega_arr[rem] + 1;
    }

    initialized = true;
}

/* Compute sopfr and omega of a number t (seq_t). */
static void factor_sopfr_omega(seq_t t, seq_t *sopfr, seq_t *omega)
{
    *sopfr = 0;
    *omega = 0;
    seq_t tmp = t;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > tmp) break;
        if (tmp % p == 0) {
            while (tmp % p == 0) {
                tmp /= p;
                *sopfr += p;
                *omega += 1;
            }
        }
    }
    if (tmp > 1) {          /* remaining factor is prime */
        *sopfr += tmp;
        *omega += 1;
    }
}

/* Ensure cache has room for index n. */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_capacity) return;
    size_t new_cap = n * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;           /* allocation failure */
    for (size_t i = cache_capacity; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init();
    ensure_cache_size(n);
    if (cache[n - 1] != 0) return cache[n - 1];

    seq_t result = 0;
    for (idx_t m = 1; ; ++m) {
        if (m == n) continue;          /* skip m = n to avoid division by zero */
        seq_t x = (m > n) ? (seq_t)(m - n) : (seq_t)(n - m);
        seq_t y = (seq_t)(m + n);
        seq_t sopfr_x, omega_x, sopfr_y, omega_y;

        if (x <= (seq_t)LIMIT_SIEVE) {
            sopfr_x = sopfr_arr[(idx_t)x];
            omega_x = omega_arr[(idx_t)x];
        } else {
            factor_sopfr_omega(x, &sopfr_x, &omega_x);
        }

        if (y <= (seq_t)LIMIT_SIEVE) {
            sopfr_y = sopfr_arr[(idx_t)y];
            omega_y = omega_arr[(idx_t)y];
        } else {
            factor_sopfr_omega(y, &sopfr_y, &omega_y);
        }

        if (sopfr_x == sopfr_y && omega_x == omega_y) {
            result = (seq_t)m;
            break;
        }
    }

    cache[n - 1] = result;
    return result;
}
