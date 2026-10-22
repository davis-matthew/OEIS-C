/*
 * A350706  Composite numbers that have no "safe prime" factors.
 * https://oeis.org/A350706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer that has no safe prime factors
 *            (a safe prime p is a prime such that (p-1)/2 is also prime)
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code uses dynamic arrays for primes and for the terms of the
    sequence.  All allocations use explicit casts so the code compiles
    as both C11 and C++17.  The auxiliary helper functions from the
    utils library are used for prime‐related operations.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Static storage for primes and terms  ---------- */

static uint64_t *prime_list = NULL;      /* primes in ascending order   */
static bool     *prime_safe = NULL;      /* true if prime is "safe"     */
static size_t    prime_cnt  = 0;         /* number of primes stored     */
static size_t    prime_cap  = 0;         /* allocated capacity          */

static seq_t *term_list = NULL;          /* sequence terms (composite)  */
static size_t term_cnt  = 0;             /* number of terms stored      */
static size_t term_cap  = 0;             /* allocated capacity          */

/* current value to test when searching for next term */
static uint64_t current_test = 4;

/* ----------  Prime list helper functions  ---------- */

/* ensure primes 2 and 3 are present in the list */
static void init_primes(void)
{
    if (prime_cnt == 0) {
        /*
         * Allocate initial capacity for the prime tables.
         */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
            prime_safe = (bool *)malloc(prime_cap * sizeof(bool));
        }
        /* add prime 2 */
        prime_list[prime_cnt]   = 2;
        prime_safe[prime_cnt]   = false;   /* 2 is not a safe prime   */
        prime_cnt++;
        /* add prime 3 */
        prime_list[prime_cnt]   = 3;
        prime_safe[prime_cnt]   = false;   /* 3 is not a safe prime   */
        prime_cnt++;
    }
}

/* grow the prime tables if necessary and add a new prime entry */
static void add_prime(uint64_t p, bool safe)
{
    if (prime_cnt == prime_cap) {
        prime_cap = (prime_cap == 0 ? 16 : prime_cap * 2);
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        prime_safe = (bool *)realloc(prime_safe, prime_cap * sizeof(bool));
    }
    prime_list[prime_cnt]   = p;
    prime_safe[prime_cnt]   = safe;
    prime_cnt++;
}

/* primality test for a candidate n using the current prime list */
static bool isPrimeSimple(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* determine if n is prime; if so, add it to the prime list
   and compute its safety flag.  */
static bool isPrimeAndAdd(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    /* n is prime, determine if it is safe */
    bool safe = false;
    if (n > 2 && (n & 1ULL)) {
        uint64_t half = (n - 1) / 2;
        if (half >= 2 && isPrimeSimple(half)) safe = true;
    }
    add_prime(n, safe);
    return true;
}

/* ----------  Helper to test for safe prime factor  ---------- */

/* true if the composite number n has at least one safe prime factor */
static bool hasSafePrimeFactor(uint64_t n)
{
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            if (prime_safe[i]) return true;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                      /* remaining prime factor */
        bool safe = false;
        if (temp > 2 && (temp & 1ULL)) {
            uint64_t half = (temp - 1) / 2;
            if (half >= 2 && isPrimeSimple(half)) safe = true;
        }
        if (safe) return true;
    }
    return false;
}

/* ----------  Ensure enough terms are precomputed  ---------- */

static void ensure_terms_upto(idx_t n)
{
    init_primes();                       /* add 2 and 3 if needed */
    while (term_cnt < (size_t)n) {
        bool prime = isPrimeAndAdd(current_test);
        if (!prime && !hasSafePrimeFactor(current_test)) {
            if (term_cnt == term_cap) {
                term_cap = (term_cap == 0 ? 16 : term_cap * 2);
                term_list = (seq_t *)realloc(term_list, term_cap * sizeof(seq_t));
            }
            term_list[term_cnt++] = (seq_t)current_test;
        }
        ++current_test;
    }
}

/* ----------  Public sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* indices less than 1 are undefined */
    ensure_terms_upto(n);
    return term_list[n - 1];
}
