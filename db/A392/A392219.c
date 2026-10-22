/*
 * A392219  Primes p such that the Chebyshev distance from 1 to p in the Ulam
 *          spiral is a prime number.
 * https://oeis.org/A392219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that floor((sqrt(p-1)+1)/2) is prime
 *            (computed by searching primes and testing the derived value)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

/* ------------------------------------------------------------------ */
/*  Static state                                                    */
/* ------------------------------------------------------------------ */
static idx_t *prime_list      = NULL;   /* primes found so far */
static size_t prime_count     = 0;        /* number of primes stored   */
static size_t prime_cap       = 0;        /* allocated capacity       */
static idx_t next_candidate   = 2;        /* next number to test */

static seq_t *seq_vals        = NULL;   /* qualifying sequence values */
static size_t seq_len         = 0;        /* number of values stored  */
static size_t seq_cap         = 0;        /* allocated capacity       */

/* ------------------------------------------------------------------ */
/*  Utilities for dynamic arrays                                     */
/* ------------------------------------------------------------------ */
static void ensure_prime_cap(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void ensure_seq_cap(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Prime testing using the stored prime list                         */
/* ------------------------------------------------------------------ */
static bool is_prime_using_list(idx_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > x / p) break;          /* avoid overflow */
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Generate the next prime number and append it to prime_list      */
/* ------------------------------------------------------------------ */
static idx_t next_prime(void) {
    while (true) {
        /* Skip even numbers after 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0) {
            ++next_candidate;
            continue;
        }
        if (is_prime_using_list(next_candidate)) {
            ensure_prime_cap(prime_count + 1);
            prime_list[prime_count++] = next_candidate;
            idx_t prime = next_candidate;
            ++next_candidate;
            return prime;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public routine: a(idx_t n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for offset 0 */

    /* Generate sequence values until we have at least n terms */
    while (seq_len < n) {
        idx_t p_val = next_prime();           /* next prime candidate */
        seq_t p = (seq_t)p_val;               /* cast to seq_t */

        /* Compute m = floor((sqrt(p-1)+1)/2) */
        seq_t t = floor_sqrt(p - 1);
        seq_t m = (t + 1) >> 1;               /* divide by 2 safely */

        /* Test whether m itself is prime */
        if (is_prime_big(m)) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = p;
        }
    }
    return seq_vals[n - 1];
}
