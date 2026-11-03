/*
 * A253388  Numbers n such that the number of divisors of n is the product of
 *          two distinct primes.
 * https://oeis.org/A253388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 4902, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9724 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253388: a(n) = the n-th positive integer whose number of divisors τ(n)
   is the product of two distinct primes.  The function generates the
   sequence on demand, storing previously found terms in a static cache.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_divisors.h"
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

/* Helper: test whether a small integer d is a product of two distinct primes. */
static bool is_product_two_distinct_primes(uint64_t d)
{
    if (d < 6)            /* 2*3 is the smallest possibility */
        return false;
    /* If d itself is prime, it cannot be a product of two primes. */
    if (is_prime_seq((seq_t)d))
        return false;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            uint64_t q = d / p;
            if (p != q && is_prime_seq((seq_t)p) && is_prime_seq((seq_t)q))
                return true;
        }
    }
    return false;
}

/* The exported sequence function. */
seq_t a(idx_t n)
{
    /* Static state shared between calls */
    static seq_t *terms = NULL;          /* cached terms */
    static size_t terms_len = 0;         /* elements currently stored */
    static size_t terms_cap = 0;         /* capacity of the array */
    static uint64_t scanned_max = 1;     /* largest integer examined so far */

    /* If n is zero or negative (should not happen with unsigned idx_t),
       just return zero.  */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        scanned_max++;   /* next candidate */

        /* Compute τ(scanned_max) */
        uint64_t tau = num_divisors(scanned_max);

        /* Test the divisor‑count condition. */
        if (is_product_two_distinct_primes(tau)) {
            /* Append the candidate to the cache. */
            ensure_capacity(terms_len + 1, &terms_cap,
                            OEISC_VPP(&terms), sizeof(seq_t));
            terms[terms_len++] = (seq_t)scanned_max;
        }
    }

    return terms[n - 1];
}
