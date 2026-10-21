/*
 * A242976  Number of ways to write n = i + j + k with 0 < i <= j <= k such
 *          that prime(i) mod i, prime(j) mod j and prime(k) mod k are all
 *          triangular numbers, where prime(m) mod m denotes the least
 *          nonnegative residue of prime(m) modulo m.
 * https://oeis.org/A242976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (i, j, k) with 0 < i ≤ j ≤ k, i + j + k
 *            = n, * such that prime(i) mod i, prime(j) mod j and prime(k) mod
 *            k are all triangular. * Implements a straightforward search over
 *            all admissible triples after pre‑computing * the set of “good”
 *            indices
 * Range:     u8 n <= 6000, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Constants: limit for primes (covers the 10 000th prime 104 729)
   and maximum n required by the tests (≈10 000). */
#define LIMIT_PRIME 200000
#define MAX_N 10000

/* Prime table (zero‑based: primes[0] = 2 is the 1st prime). */
static uint32_t primes[20000];
static size_t prime_count = 0;

/* Boolean sieve of Eratosthenes up to LIMIT_PRIME. */
static bool is_prime[LIMIT_PRIME + 1];

/* Triangular numbers up to LIMIT_PRIME. */
static bool tri[LIMIT_PRIME + 1];

/* Set of indices i (1‑based) for which prime(i) % i is triangular. */
static bool good[MAX_N + 1];
static size_t good_list[MAX_N + 1];
static size_t good_count = 0;

/* One‑time initialization flag. */
static int initialized = 0;

/* Perform sieve, fill primes, build triangular table, and compute good[]/list. */
static void init_once(void)
{
    if (initialized)
        return;

    /* 1. Sieve of Eratosthenes. */
    for (int i = 0; i <= LIMIT_PRIME; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; (long long)p * p <= LIMIT_PRIME; ++p) {
        if (is_prime[p]) {
            for (int m = p * p; m <= LIMIT_PRIME; m += p)
                is_prime[m] = false;
        }
    }

    /* 2. Collect primes into the primes[] array. */
    size_t idx = 0;
    for (int i = 2; i <= LIMIT_PRIME && idx < sizeof(primes) / sizeof(primes[0]); ++i) {
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;
    }
    prime_count = idx;

    /* 3. Build triangular number table. */
    for (int i = 0; i <= LIMIT_PRIME; ++i)
        tri[i] = false;
    int t = 0;
    while ((long long)t * (t + 1) / 2 <= LIMIT_PRIME) {
        tri[t * (t + 1) / 2] = true;
        ++t;
    }

    /* 4. Compute good[] and the list of good indices. */
    good_count = 0;
    for (int i = 1; i <= MAX_N; ++i) {
        bool good_i;
        if (i == 1) {
            /* prime(1) is defined as 0, and 0 is triangular (t = 0). */
            good_i = true;
        } else {
            uint32_t rem = primes[i - 1] % i; /* i‑th prime, primes[0] is the 1st prime. */
            good_i = tri[rem];
        }
        good[i] = good_i;
        if (good_i) {
            good_list[good_count++] = (size_t)i;
        }
    }

    initialized = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0 || n > MAX_N)
        return (seq_t)0;   /* values outside the pre‑computed range are undefined. */

    init_once();

    seq_t cnt = 0;
    /* Iterate over good indices i. */
    for (size_t ii = 0; ii < good_count && good_list[ii] <= n; ++ii) {
        idx_t i = good_list[ii];
        idx_t max_j = (n - i) / 2;          /* ensures j ≤ k. */
        /* Iterate over good indices j ≥ i. */
        for (size_t jj = ii; jj < good_count && good_list[jj] <= max_j; ++jj) {
            idx_t j = good_list[jj];
            idx_t k = n - i - j;
            /* j ≤ k is guaranteed by max_j; check if k is good. */
            if (good[(size_t)k])
                ++cnt;
        }
    }
    return cnt;
}
