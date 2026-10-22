/*
 * A302505  Numbers whose prime indices are squarefree and have disjoint prime
 *          indices.
 * https://oeis.org/A302505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   The terms are defined as the n‑th positive integer whose * prime
 *            indices are squarefree and pairwise coprime. * This
 *            implementation tests integers in increasing order
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_LIMIT 2000000        /* upper bound for primes we will need */

static uint32_t  *prime_index  = NULL;  /* prime -> its (1‑based) index, 0 if not prime */
static uint32_t  *primes       = NULL;  /* list of primes found */
static size_t     prime_count  = 0;
static bool      *sqfree_index = NULL;  /* true iff index is squarefree */

/* ------------------------------------------------------------------
 * Simple sieve of Eratosthenes.  After this, prime_index[p] is valid
 * for all p <= PRIME_LIMIT, primes[] contains all primes <= PRIME_LIMIT,
 * and prime_count is the number of primes.
 * ------------------------------------------------------------------ */
static void init_primes(void)
{
    static bool built = false;
    if (built) return;
    built = true;

    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    /* first pass: sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = true;
        }
    }

    /* count primes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) ++prime_count;

    /* allocate arrays */
    primes      = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    prime_index = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    if (!primes || !prime_index) exit(EXIT_FAILURE);

    /* build lists */
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[idx] = i;
            prime_index[i] = (uint32_t)(idx + 1); /* 1‑based index */
            ++idx;
        }
    }
    free(sieve);

    /* precompute squarefree property of all indices 1..prime_count */
    sqfree_index = (bool *)calloc(prime_count + 1, sizeof(bool));
    if (!sqfree_index) exit(EXIT_FAILURE);
    for (size_t i = 1; i <= prime_count; ++i) sqfree_index[i] = true;

    for (uint32_t p = 2; (uint64_t)p * p <= prime_count; ++p) {
        uint32_t square = p * p;
        for (uint32_t m = square; m <= prime_count; m += square)
            sqfree_index[m] = false;
    }
}

/* ------------------------------------------------------------------
 * Euclidean gcd for 32‑bit unsigned integers.
 * ------------------------------------------------------------------ */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* ------------------------------------------------------------------
 * Test whether a candidate integer satisfies the definition.
 * ------------------------------------------------------------------ */
static bool is_valid(uint64_t n)
{
    if (n == 1) return true;

    uint64_t temp = n;
    uint32_t indices[32];  /* max distinct primes in a 64‑bit number is 16, safe */
    size_t idx_len = 0;

    /* trial division */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint32_t k = prime_index[p];
            if (k > 1 && exp > 1) return false;  /* repeated prime index not allowed */

            if (k > 1) {
                /* check for coprimality w/ previous indices */
                for (size_t j = 0; j < idx_len; ++j)
                    if (gcd32(k, indices[j]) > 1) return false;
                indices[idx_len++] = k;
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        uint32_t p = (uint32_t)temp;    /* temp is prime and within limit */
        uint32_t k = prime_index[p];
        if (k > 1) {
            for (size_t j = 0; j < idx_len; ++j)
                if (gcd32(k, indices[j]) > 1) return false;
            indices[idx_len++] = k;
        }
    }

    /* squarefree test for all indices encountered */
    for (size_t i = 0; i < idx_len; ++i)
        if (!sqfree_index[indices[i]]) return false;

    return true;
}

/* ------------------------------------------------------------------
 * Sequence generation with memoisation.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, guard against caller error */

    /* static state */
    static seq_t  *cache       = NULL;
    static size_t  cache_cap   = 0;
    static size_t  cache_len   = 0;
    static uint64_t cand       = 1;  /* next integer to test */

    init_primes();

    while (cache_len < n) {
        if (is_valid(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
        ++cand;
    }
    return cache[n - 1];
}
