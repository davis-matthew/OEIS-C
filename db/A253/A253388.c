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
 * Helpers:   divisor_count_u64, ensure_capacity, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253388: a(n) = the n-th positive integer whose number of divisors τ(n)
   is the product of two distinct primes.  The function generates the
   sequence on demand, storing previously found terms in a static cache.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/isPrimeTrial.h"
#include "../utils/ensure_capacity.h"

/* Helper: test whether a small integer d is a product of two distinct primes. */
static bool is_product_two_distinct_primes(uint64_t d)
{
    if (d < 6)            /* 2*3 is the smallest possibility */
        return false;
    /* If d itself is prime, it cannot be a product of two primes. */
    if (isPrimeTrial((seq_t)d))
        return false;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            uint64_t q = d / p;
            if (p != q && isPrimeTrial((seq_t)p) && isPrimeTrial((seq_t)q))
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
        uint64_t tau = divisor_count_u64(scanned_max);

        /* Test the divisor‑count condition. */
        if (is_product_two_distinct_primes(tau)) {
            /* Append the candidate to the cache. */
            ensure_capacity(terms_len + 1, &terms_cap,
                            (void **)&terms, sizeof(seq_t));
            terms[terms_len++] = (seq_t)scanned_max;
        }
    }

    return terms[n - 1];
}
