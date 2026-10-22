/*
 * A336378  Numbers k such that gcd(k, prime(k-1) + prime(k+1)) = 1.
 * https://oeis.org/A336378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that gcd(k, prime(k-1) +
 *            prime(k+1)) = 1. The first terms are a(1)=2, a(2)=3, a(3)=7,
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses internal dynamic prime
   generation (trial division) and self‑memoised
   storage of the sequence values. It works for
   n up to at least 10000 quickly. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint64_t *prime_array   = NULL; /* array of primes */
static size_t    prime_count   = 0;     /* number of primes stored */
static uint64_t  next_candidate = 3;    /* next odd number to test */

/* ensure at least idx primes (1‑based) are available */
static void ensure_primes_upto_index(size_t idx)
{
    while (prime_count < idx) {
        /* initialise the list with the first prime 2 */
        if (prime_count == 0) {
            prime_array = (uint64_t *)realloc(NULL, sizeof(uint64_t));
            prime_array[0] = 2;
            prime_count = 1;
            continue;
        }

        /* test next_candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_array[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            prime_array = (uint64_t *)realloc(prime_array,
                                              (prime_count + 1) * sizeof(uint64_t));
            prime_array[prime_count++] = next_candidate;
        }
        next_candidate += 2; /* only odd candidates needed */
    }
}

static seq_t  *term_arr   = NULL; /* memoised sequence values */
static size_t  term_cnt   = 0;     /* number of terms stored   */
static size_t  term_cap   = 0;     /* allocated capacity       */
static idx_t   max_checked_k = 1; /* highest k already tested   */

/* dynamic array helpers */
static void grow_terms(size_t min_cap)
{
    if (term_cap >= min_cap)
        return;
    size_t new_cap = term_cap == 0 ? 16 : term_cap * 2;
    if (new_cap < min_cap)
        new_cap = min_cap;
    term_arr = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
    term_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined by the problem statement, return 0 */
        return 0;

    /* grow list until we have at least n terms */
    while (term_cnt < (size_t)n) {
        idx_t k = max_checked_k + 1; /* next k to test */
        /* ensure primes are available for indices k-1 and k+1 */
        ensure_primes_upto_index((size_t)(k + 1));

        /* primes are 1‑based: prime(k-1) = primes[k-2] */
        uint64_t p_prev = prime_array[(size_t)(k - 2)];
        uint64_t p_next = prime_array[(size_t)(k)];
        uint64_t sum = p_prev + p_next;

        seq_t g = gcd((seq_t)k, (seq_t)sum);
        if (g == 1) {
            grow_terms(term_cnt + 1);
            term_arr[term_cnt++] = (seq_t)k;
        }
        max_checked_k = k;
    }
    return term_arr[(size_t)n - 1];
}
