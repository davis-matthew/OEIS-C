/*
 * A327414  Minimal prime partition representation of even integers.
 * https://oeis.org/A327414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n, p) where p is the greatest prime * less than
 *            2n−1 (i.e., the greatest prime ≤ 2n−2). a(0)=0, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- internal state for prime sieving ---------- */
static uint64_t sieve_max = 0;          /* largest value already sieved   */
static uint8_t *is_prime = NULL;        /* primality flags                */
static uint64_t *prime_list = NULL;     /* list of primes in ascending order */
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Expand the sieve to at least 'limit'.  On first call the sieve
 * starts at 2 and grows as required.  The implementation uses the
 * classic O(n log log n) algorithm. */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_max) return;          /* already enough */

    if (limit < 2) limit = 2;

    /* Allocate new flags array */
    uint8_t *new_flags = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_flags) exit(EXIT_FAILURE);
    if (is_prime) free(is_prime);
    is_prime = new_flags;

    /* Initialise all as prime, then clear 0 and 1 */
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Build list of primes */
    size_t est = (size_t)(limit / (log((double)limit) + 1.0));
    if (est > prime_cap) {
        size_t new_cap = est * 2;
        uint64_t *new_list = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
        if (!new_list) exit(EXIT_FAILURE);
        if (prime_list) free(prime_list);
        prime_list = new_list;
        prime_cap = new_cap;
    }

    prime_count = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                                   new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_count++] = i;
        }
    }

    sieve_max = limit;
}

/* Compute the binomial coefficient C(n,k) using the multiplicative
 * formula.  The division is always exact at every step. */
static seq_t binom(uint64_t n, uint64_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;        /* use symmetry */

    seq_t result = (seq_t)1;
    for (uint64_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* ---------- OEIS sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    uint64_t two_n = 2 * (uint64_t)n;          /* 2n as a 64‑bit integer   */

    /* Ensure we have primes up to 2n-2 (the largest candidate for p). */
    ensure_sieve(two_n - 2);

    uint64_t p = two_n - 2;                    /* start just below 2n-1     */
    while (p >= 2 && !is_prime[p]) --p;
    if (p < 2) p = 2;                         /* safety net – never hit here */

    return binom(two_n, p);
}
