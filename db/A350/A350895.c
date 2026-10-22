/*
 * A350895  a(n) = 1 + a(n-1) * prime(n), starting a(0) = 1.
 * https://oeis.org/A350895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = 1 + a(n-1) * prime(n)   (prime(1)=2)      */

/* This implementation lazily computes primes and sequence values,
   remembering all values up to the largest requested index.  All
   computations are performed with the type seq_t, which may be an
   arbitrary‑precision integer or a fixed‑size unsigned type.  The
   algorithm is O(n) with a very small constant factor, well under
   two seconds for n≈10000.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following global tables are expanded on demand.  */
static uint64_t *prime_cache   = NULL; /* prime_cache[0] = 2       */
static size_t   prime_cap     = 0;     /* allocated capacity       */
static idx_t    prime_count   = 0;     /* number of primes stored  */

static seq_t   *a_cache       = NULL; /* a_cache[i] = a(i)        */
static size_t   a_cap         = 0;     /* allocated capacity       */
static idx_t    a_count       = 0;     /* number of a values stored*/

/* ------------------------------------------------------------------ */
/* Ensure prime_cache has room for at least 'needed' entries.         */
/* ------------------------------------------------------------------ */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Generate successive primes until prime_count >= target_count.     */
/* ------------------------------------------------------------------ */
static void generate_primes_up_to(idx_t target_count)
{
    if (prime_count >= target_count)
        return;

    uint64_t candidate = (prime_count == 0) ? 2 : prime_cache[prime_count - 1] + 1;
    while (prime_count < target_count) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_capacity(prime_count + 1);
            prime_cache[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure a_cache has room for at least 'needed' entries.             */
/* ------------------------------------------------------------------ */
static void ensure_a_capacity(size_t needed)
{
    if (a_cap >= needed)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute a(i) for all indices up to target_n (inclusive).  Any     */
/* already computed values are left untouched.                        */
/* ------------------------------------------------------------------ */
static void compute_a_up_to(idx_t target_n)
{
    /* no work needed if everything is already computed */
    if (a_count > 0 && target_n < a_count)
        return;

    /* Allocate storage for a_cache if this is the first call */
    if (a_cache == NULL) {
        ensure_a_capacity(target_n + 1);
        a_cache[0] = (seq_t)1; /* a(0) = 1 */
        a_count    = 1;
    } else {
        /* grow a_cache if required by the new request */
        ensure_a_capacity(target_n + 1);
    }

    /* Make sure we have at least target_n primes available */
    generate_primes_up_to(target_n);

    /* Compute the missing sequence values */
    for (idx_t i = a_count; i <= target_n; ++i) {
        seq_t prev = a_cache[i - 1];
        uint64_t p  = prime_cache[i - 1];   /* prime(i)  (1‑based) */
        a_cache[i] = (seq_t)1 + prev * (seq_t)p;
    }

    a_count = target_n + 1; /* next uncomputed index */
}

/* ------------------------------------------------------------------ */
/* Public API: returns a(n) for any n ≥ 0 in O(n) time.                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_a_up_to(n);
    return a_cache[n];
}
