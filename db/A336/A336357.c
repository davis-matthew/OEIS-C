/*
 * A336357  Numbers k such that sigma(k) has fewer distinct prime factors than
 *          k.
 * https://oeis.org/A336357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that the number of distinct prime
 *            factors of σ(k) is strictly smaller than the number of distinct
 *            prime factors of k
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   count_distinct_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/sigma_of.h"

/* Return the number of distinct prime factors of a 64‑bit integer. */
static inline int omega64(uint64_t n)
{
    return count_distinct_primes(n);
}

/* Test whether k satisfies the defining condition. */
static inline bool good_k(uint64_t k)
{
    if (k <= 1u)   /* σ(1)=1 has 0 distinct primes, ω(1)=0 */
        return false;

    int omega_k   = omega64(k);

    seq_t sig = sigma_of(k);
    /* For the range of n considered (≤ 10 000) σ(k) fits in 64 bits. */
    uint64_t sig64 = (uint64_t)sig;
    int omega_sig = omega64(sig64);

    return omega_sig < omega_k;
}

/* Static cache of sequence terms. */
static seq_t *terms   = NULL;   /* array of terms */
static size_t len    = 0;       /* number of terms stored */
static size_t cap    = 0;       /* allocated capacity */
static uint64_t next_k = 2;     /* next integer to test (start from 2) */

/* Append a new term, expanding the array if necessary. */
static void append_term(uint64_t term)
{
    if (len >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – terminate with no output. */
            abort();
        }
        terms = tmp;
        cap   = new_cap;
    }
    terms[len++] = (seq_t)term;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    /* Grow the sequence until we have at least n terms. */
    while (len < n) {
        if (good_k(next_k))
            append_term(next_k);
        ++next_k;
    }
    return terms[n - 1];
}
