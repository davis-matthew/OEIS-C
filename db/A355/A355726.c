/*
 * A355726  a(n) = a(n-2) + prime(n-1) for a(0) = a(1) = 0.
 * https://oeis.org/A355726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + prime(n-1); a(0)=a(1)=0
 * Range:     u8 n <= 18, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A355726 using an incremental prime sieve and memoisation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ---------- Prime generation helpers ---------- */

static uint64_t *primes = NULL;        /* array of primes, primes[0] = 2 */
static size_t   prime_len = 0;         /* number of primes stored */
static size_t   prime_cap = 0;         /* allocated capacity for primes */
static uint64_t prime_candidate = 2;   /* next candidate to test */

/* Append a new prime to the array, growing capacity if necessary */
static void prime_append(uint64_t p)
{
    if (prime_cap == 0) {
        prime_cap = 8;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_len == prime_cap) {
        prime_cap <<= 1;
        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
    }
    primes[prime_len++] = p;
}

/* Test whether a number is prime using the primes already known */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure at least 'needed' primes are stored (needed is a count, not an index) */
static void ensure_primes_count(size_t needed)
{
    if (prime_len >= needed) return;

    /* Generate primes until we have enough */
    while (prime_len < needed) {
        if (prime_candidate == 2) {
            prime_append(2);
            prime_candidate = 3;
            continue;
        }

        if (is_prime_candidate(prime_candidate)) {
            prime_append(prime_candidate);
        }
        prime_candidate += 2; /* skip even numbers after 2 */
    }
}

/* ---------- Sequence memoisation helpers ---------- */

static seq_t *seq_cache = NULL;   /* cached a(n) values */
static size_t  seq_len   = 0;     /* number of values currently cached */
static size_t  seq_cap   = 0;     /* allocated capacity for seq_cache */

/* Grow seq_cache to at least 'cap' elements */
static void ensure_seq_cache_cap(size_t cap)
{
    if (seq_cap >= cap) return;

    seq_t *new_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (seq_cache && seq_len > 0) {
        memcpy(new_cache, seq_cache, seq_len * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = new_cache;
    seq_cap   = cap;
}

/* Compute all a(n) up to the requested index 'n' (inclusive) */
static void ensure_seq_cache(idx_t n)
{
    if (seq_len > n) return;                 /* already computed */

    if (n >= 2) {
        /* Need primes[0 .. n-2], i.e. (n-1) primes in total */
        ensure_primes_count((size_t)(n - 1));
    }

    ensure_seq_cache_cap((size_t)(n + 1));

    for (idx_t i = seq_len; i <= n; ++i) {
        if (i < 2) {
            seq_cache[i] = 0;                /* a(0)=a(1)=0 */
        } else {
            seq_cache[i] = seq_cache[i - 2] + (seq_t)primes[i - 2];
        }
    }
    seq_len = (size_t)(n + 1);
}

/* ---------- Public API ---------- */

seq_t a(idx_t n)
{
    ensure_seq_cache(n);
    return seq_cache[n];
}
