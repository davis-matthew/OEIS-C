/*
 * A261387  Number of ways to write n = k + m with 0 < k < m < n such that
 *          prime(k) is a primitive root modulo prime(m) and also prime(m) is
 *          a primitive root modulo prime(k).
 * https://oeis.org/A261387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (k,m) with 0<k<m<n such that prime(k) is
 *            a primitive root modulo prime(m) and prime(m) is a primitive
 *            root modulo prime(k). Computed by exhaustive search. The sieve
 *            precomputes the first few primes and their smallest prime
 *            factors; the search loops over k ≤ (n-1)/2 for each n
 * Range:     u8 n <= 2208, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   mod_pow_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow_64.h"

/* Types supplied by the build */
typedef uint64_t idx_t;   /* index type */

/* --------------- Sieve data ------------------------------------ */
#define SIEVE_LIMIT 200000   /* covers first ~17984 primes, enough for n≈10000 */
static uint64_t  *prime_arr = NULL;   /* 1‑based: prime_arr[i] = i‑th prime         */
static size_t     prime_cnt  = 0;     /* number of primes found                   */
static uint32_t  *spf        = NULL;  /* smallest prime factor for each n          */

/* --------------- Primitive–root test for a prime p ------------- */
static bool is_primitive_root(uint64_t g, uint64_t p) {
    if (p == 2) return true;   /* 1 is the only unit modulo 2 */
    uint64_t phi = p - 1;
    uint64_t x = phi;
    uint64_t prev_q = 0;
    while (x > 1) {
        uint64_t q = spf[(size_t)x];
        if (q == 0) q = x;                 /* prime itself */
        if (q != prev_q) {                 /* distinct prime factor only */
            if (mod_pow_64(g, phi / q, p) == 1) return false;
            prev_q = q;
        }
        while (x % q == 0) x /= q;
    }
    return true;
}

/* --------------- Initialise the sieve --------------------------------- */
static void init_sieve(void) {
    if (prime_arr) return;   /* already initialised */

    prime_arr = (uint64_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint64_t));
    spf       = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime                */
            spf[i] = i;
            prime_arr[++prime_cnt] = i;
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                if (spf[(size_t)j] == 0) spf[(size_t)j] = i;
        }
    }
}

/* --------------- Cache for a(n) ------------------------------------ */
static seq_t *a_cache    = NULL;   /* cache[0..n] inclusive           */
static idx_t  cache_size = 0;      /* allocated size (max index + 1)   */

static seq_t compute_a_value(idx_t n) {
    if (n <= 2) return 0;   /* definition for n=1,2 */

    seq_t count = 0;
    idx_t half = (n - 1) / 2;
    for (idx_t k = 1; k <= half; ++k) {
        idx_t m = n - k;   /* ensures k < m < n               */
        uint64_t pk = prime_arr[k];
        uint64_t pm = prime_arr[m];
        if (!is_primitive_root(pk, pm)) continue;
        if (!is_primitive_root(pm, pk)) continue;
        ++count;
    }
    return count;
}

/* --------------- exported function --------------------------------- */
seq_t a(idx_t n) {
    init_sieve();

    if (n == 0) return 0;   /* safety for index 0                */

    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) exit(1);        /* allocation failure */
        a_cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i)
            a_cache[i] = compute_a_value(i);
        cache_size = n + 1;
    }

    return a_cache[n];
}
