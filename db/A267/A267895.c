/*
 * A267895  Numbers whose number of odd divisors is prime.
 * https://oeis.org/A267895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267895: Numbers whose number of odd divisors is prime. */
/* The algorithm generates numbers in increasing order, counting odd
 * divisors by prime factorisation of the odd part, and checks whether
 * the divisor count is a prime number. It uses on‑demand prime
 * generation and memoises results so that subsequent calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a small integer (fits in 64 bits) is prime.
 * Works for values up to at least 2^32, which is more than enough
 * for divisor counts encountered in this sequence. */
static bool small_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

/* Global dynamic list of primes (2,3,5,7,...) used for factorisation. */
static size_t     primes_cnt   = 0;
static size_t     primes_cap   = 0;
static uint64_t*  primes       = NULL;   /* array of primes */
static uint64_t   prime_candidate = 3;   /* next odd number to test for primality */

/* Ensure that primes are available up to at least 'limit'.
 * The list is expanded only as required. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_cnt == 0) {          /* initialise */
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
    }
    while (prime_candidate <= limit) {
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > prime_candidate) break;
            if (prime_candidate % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = prime_candidate;
        }
        prime_candidate += 2;
    }
}

/* Return true iff the integer n has a prime number of odd divisors. */
static bool has_prime_odd_divcount(uint64_t n)
{
    uint64_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;   /* strip powers of two */
    if (odd == 1) return false;        /* odd part is 1 -> only one odd divisor */

    uint64_t rem = odd;
    uint64_t divisor_cnt = 1;

    /* Ensure we have primes up to sqrt(rem).  Since rem <= n, this is safe. */
    ensure_primes_up_to(rem);

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            int exp = 0;
            do { rem /= p; ++exp; } while (rem % p == 0);
            divisor_cnt *= (uint64_t)(exp + 1);
        }
    }
    if (rem > 1) divisor_cnt *= 2;     /* last prime factor with exponent 1 */

    return small_prime(divisor_cnt);
}

/* The public sequence function. */
seq_t a(idx_t n)
{
    static seq_t*  cache   = NULL;   /* memoised sequence values */
    static size_t  cache_len = 0;     /* number of values cached */
    static size_t  cache_cap = 0;     /* allocated capacity */
    static uint64_t last_checked = 1; /* last integer examined */

    if (n == 0) return 0;  /* undefined for offset 1, but safe guard */

    /* Generate terms until we have at least n values. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (has_prime_odd_divcount(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
