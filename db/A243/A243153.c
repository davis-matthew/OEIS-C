/*
 * A243153  Larger of two consecutive primes whose difference is a semiprime.
 * https://oeis.org/A243153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A243153
 *  Larger of two consecutive primes whose difference is a semiprime.
 *  The function a(idx_t n) returns the n‑th term (1‑based index).
 *  The implementation builds primes incrementally and stores all
 *  qualifying primes.  Each call only generates as many terms as
 *  necessary; the function is memoised and works for arbitrary call order.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

static seq_t *terms = NULL;      /* sequence values a(n)    */
static size_t terms_len = 0;     /* number of terms stored  */
static size_t terms_cap = 0;     /* allocated capacity      */

static idx_t *primes = NULL;     /* list of primes seen     */
static size_t primes_len = 0;    /* number of primes seen   */
static size_t primes_cap = 0;    /* allocated capacity for primes */

/* Ensure that the terms array has capacity for at least 'need' elements */
static void ensure_terms_cap(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t newcap = (terms_cap == 0) ? 64 : terms_cap * 2;
    while (newcap < need)
        newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!newptr)
        abort();
    terms = newptr;
    terms_cap = newcap;
}

/* Ensure that the primes array has capacity for at least 'need' elements */
static void ensure_primes_cap(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t newcap = (primes_cap == 0) ? 64 : primes_cap * 2;
    while (newcap < need)
        newcap *= 2;
    idx_t *newptr = (idx_t *)realloc(primes, newcap * sizeof(idx_t));
    if (!newptr)
        abort();
    primes = newptr;
    primes_cap = newcap;
}

/* Test whether n is prime using the already known primes */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if ((idx_t)(p * p) > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Test whether x (>= 2) is a semiprime: product of exactly two primes */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)            /* 2 and 3 are not semiprime */
        return false;
    uint64_t p = _smallest_prime_factor(x);
    if (p == 0 || p == x)
        return false;       /* 1 or prime */
    uint64_t q = x / p;
    if (q == 1)
        return false;
    uint64_t qpf = _smallest_prime_factor(q);
    return (qpf == q);      /* q is prime */
}

/* Generate primes and sequence terms until at least 'need' terms exist */
static void generate_until(size_t need)
{
    while (terms_len < need) {
        /* Generate next prime */
        idx_t cand;
        if (primes_len == 0)
            cand = 2;
        else {
            cand = primes[primes_len - 1] + 1;
            if (cand <= 2)
                cand = 3;
        }
        /* Skip even numbers >2 */
        if (cand > 2 && (cand & 1) == 0)
            cand++;
        for (;;) {
            if (is_prime_candidate(cand))
                break;
            cand++;
            if (cand > 2 && (cand & 1) == 0)   /* keep odd */
                cand++;
        }
        /* Store new prime */
        ensure_primes_cap(primes_len + 1);
        primes[primes_len++] = cand;

        /* Check the difference with the previous prime */
        if (primes_len >= 2) {
            uint64_t diff = cand - primes[primes_len - 2];
            if (is_semiprime_uint64(diff)) {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = (seq_t)cand;
            }
        }
    }
}

/* Public API required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* out of range request */
    generate_until((size_t)n);
    return terms[(size_t)n - 1];
}
