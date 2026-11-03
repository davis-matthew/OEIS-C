/*
 * A352014  a(n) = Sum_{p|n, p prime} (-1)^(n/p+1) * (n-1)!/(p-1)!.
 * https://oeis.org/A352014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{p|n, p prime} (-1)^(n/p+1) * (n-1)!/(p-1)!
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Cached factorials: fact[i] = i!   (i ranges from 0 up to the maximum n-1 requested) */
static seq_t *fact = NULL;
static size_t fact_len = 0;   /* number of entries currently initialized */
static size_t fact_cap = 0;   /* allocated capacity */

/* Cached list of primes up to the maximum n requested */
static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure fact array contains factorials up to (n-1)! */
static void ensure_fact(idx_t n) {
    size_t needed = (size_t)(n);      /* need indices 0 .. n-1 inclusive (n elements) */
    if (fact_cap < needed) {
        ensure_capacity(needed, &fact_cap, OEISC_VPP(&fact), sizeof(seq_t));
    }
    for (size_t i = fact_len; i < needed; ++i) {
        if (i == 0) {
            fact[0] = (seq_t)1;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
    }
    fact_len = needed;
}

/* Ensure prime_list contains all primes up to n */
static void ensure_primes(idx_t n) {
    if (n <= prime_limit) return;
    free(prime_list);
    prime_list = NULL;
    prime_list_cnt = 0;
    build_primes_upto(n, &prime_list, &prime_list_cnt);
    prime_limit = n;
}

/* Compute the n-th term of OEIS A352014 */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;          /* not part of the sequence, but safe */
    if (n == (idx_t)1) return 0;          /* a(1)=0 */

    /* Pre‑compute factorials and primes needed for this n */
    ensure_fact(n);                      /* factorials up to (n-1)! are ready */
    ensure_primes(n);                    /* primes up to n are ready */

    seq_t result = (seq_t)0;
    seq_t fact_n_minus1 = fact[(size_t)(n - 1)];
    unsigned long long temp = (unsigned long long)n; /* use 64‑bit for factorization */

    /* iterate over known primes up to sqrt(temp) */
    for (size_t idx = 0; idx < prime_list_cnt; ++idx) {
        uint64_t p = prime_list[idx];
        if (p * p > temp) break;
        if (temp % p == 0ULL) {
            unsigned long long e = n / p;
            seq_t sign = ((e + 1ULL) & 1ULL) ? (seq_t)(-1) : (seq_t)1;
            seq_t term = fact_n_minus1 / fact[(size_t)(p - 1)];
            result += sign * term;
            while (temp % p == 0ULL) temp /= p;
        }
    }

    /* If remaining temp is a prime > sqrt(n) */
    if (temp > 1ULL) {
        uint64_t p = temp;
        unsigned long long e = n / p;
        seq_t sign = ((e + 1ULL) & 1ULL) ? (seq_t)(-1) : (seq_t)1;
        seq_t term = fact_n_minus1 / fact[(size_t)(p - 1)];
        result += sign * term;
    }

    return result;
}
