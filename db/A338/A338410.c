/*
 * A338410  Primes p such that (p+2)/3 and (p+3)/2 are prime.
 * https://oeis.org/A338410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that (p+2)/3 and (p+3)/2 are prime. The
 *            function searches primes congruent to 7 (mod 12), keeps the
 *            qualifying ones in a static cache, and returns the n‑th term
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for the sequence values */
static seq_t *seq_vals   = NULL;
static size_t seq_cap    = 0;   /* capacity of seq_vals */
static size_t seq_len    = 0;   /* number of elements stored */

/* prime sieve cache */
static uint64_t *prime_list = NULL;
static size_t primes_cap    = 0;
static size_t primes_len    = 0;
static uint64_t primes_limit = 0;  /* largest number for which primes are known */

/* next 12k+7 candidate to test */
static uint64_t next_candidate = 7;

/* --------- integer square root (Newton iteration) -------------------------------- */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* --------- dynamic array helpers ----------------------------------------------- */
static void grow_prime_array(void)
{
    if (primes_len == primes_cap) {
        primes_cap = primes_cap ? primes_cap << 1 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, primes_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
    }
}

static void grow_seq_array(void)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
    }
}

/* --------- generate all primes up to 'limit' inclusive --------------------------- */
static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;

    if (!prime_list) {
        primes_cap = 16;
        prime_list = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!prime_list) abort();
    }

    if (primes_len == 0) {
        prime_list[primes_len++] = 2;
        primes_limit = 2;
    }

    uint64_t num = primes_limit + 1;
    if ((num & 1ULL) == 0) ++num;           /* start with the next odd */
    for (; num <= limit; num += 2) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = prime_list[i];
            if ((p * p) > num) break;
            if (num % p == 0) { prime = false; break; }
        }
        if (prime) {
            grow_prime_array();
            prime_list[primes_len++] = num;
        }
    }
    primes_limit = limit;
}

/* --------- primality test for uint64_t ---------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2;

    uint64_t limit = isqrt_u64(n);
    if (limit > primes_limit) generate_primes_up_to(limit);

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --------- main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    if (!seq_vals) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_vals) abort();
    }

    while (seq_len < n) {
        if (is_prime_uint64(next_candidate)) {
            uint64_t q1 = (next_candidate + 2) / 3;
            uint64_t q2 = (next_candidate + 3) / 2;
            if (is_prime_uint64(q1) && is_prime_uint64(q2)) {
                grow_seq_array();
                seq_vals[seq_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 12;   /* stay in the 12k+7 class */
    }
    return seq_vals[n - 1];
}
