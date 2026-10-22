/*
 * A360357  Numbers k such that k and k+1 are both products of primes of
 *          nonprime index (A320628).
 * https://oeis.org/A360357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that both k and k+1 are products of primes
 * whose indices in the prime list are nonprime (A320628).  The function
 * builds the sequence on-demand and caches the terms so that calls in any
 * order are efficient.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve up to MAX_LIMIT --------------------------------- */
#define MAX_LIMIT 5000000UL          /* upper bound for prime searching */

static bool composite[MAX_LIMIT + 1];
static uint32_t primes[MAX_LIMIT / 10];
static size_t  prime_count = 0;

/* true if index is a prime (1‑based) */
static bool idx_prime[MAX_LIMIT / 10 + 1];

/* Build the prime list and the sieve of prime indices once */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Sieve of Eratosthenes for primes up to MAX_LIMIT */
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }

    /* Sieve for prime indices: indices 0 and 1 are nonprime */
    for (size_t i = 0; i <= prime_count; ++i) idx_prime[i] = false;
    for (size_t i = 2; i <= prime_count; ++i) idx_prime[i] = true;
    for (uint32_t i = 2; i * i <= prime_count; ++i) {
        if (idx_prime[i]) {
            for (uint32_t j = i * i; j <= prime_count; j += i)
                idx_prime[j] = false;
        }
    }
}

/* Binary search for the 1‑based index of prime p (assumes p <= MAX_LIMIT) */
static uint32_t prime_index(uint32_t p)
{
    size_t l = 0, r = prime_count;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (primes[m] < p)
            l = m + 1;
        else
            r = m;
    }
    return (uint32_t)(l + 1);   /* indices are 1‑based */
}

/* Is n a product of primes whose indices are nonprime? */
static bool product_of_nonprime_index(uint64_t n)
{
    if (n <= 1) return true;                /* 1 is allowed */

    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            if (idx_prime[i + 1]) return false;  /* prime index is prime */
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        /* Remaining factor is a prime > sqrt(n).  It must be <= MAX_LIMIT. */
        if (tmp > MAX_LIMIT) return false;       /* cannot determine */
        uint32_t idx = prime_index((uint32_t)tmp);
        if (idx_prime[idx]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/* Storage for cached sequence values                                       */
static seq_t *seq      = NULL;    /* array of found terms */
static size_t seq_len  = 0;       /* number of stored terms */
static size_t seq_cap  = 0;       /* current allocated capacity */
static uint64_t next_candidate = 1;  /* smallest k not yet tested */

/* Grow the sequence array to accommodate `new_needed` elements. */
static void grow_seq(size_t new_needed)
{
    if (new_needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    while (new_cap < new_needed) new_cap <<= 1;

    /* realloc must be cast explicitly in C++ */
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* abort on allocation failure */
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/* The required function                                                     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */

    init_primes();  /* ensure primes and index sieve are ready */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t k = next_candidate;

        if (product_of_nonprime_index(k) && product_of_nonprime_index(k + 1)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)k;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
