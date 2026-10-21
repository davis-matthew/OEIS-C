/*
 * A247177  Primes p with property that the sum of the squares of the
 *          successive gaps between primes <= p is a prime number.
 * https://oeis.org/A247177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 262, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247177:
   a(n) = the nth prime p such that
          sum_{i=1}^{k-1} (p_{i+1}-p_i)^2 is prime,
   where p_k = p.  The sequence starts with a(1)=5.  The values are
   generated on demand and cached. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Access to the helper functions provided by the build system */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start); */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */

static seq_t *terms   = NULL;   /* cached sequence terms */
static size_t terms_cnt = 0;    /* number of terms cached so far */
static size_t terms_cap = 0;    /* allocated capacity */

/* Current state of the incremental prime/gap sum generator */
static uint64_t  prev_prime = 0;    /* last prime processed (0 => not started) */
static seq_t    current_sum = 0;    /* cumulative sum of squared gaps */

/* Expand the terms array to at least 'need' elements. */
static void ensure_terms_cap(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc((void *)terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    /* If we already have the requested term, return it. */
    if (terms_cnt >= (size_t)n)
        return terms[n - 1];

    /* Initialise the generator if this is the first call. */
    if (prev_prime == 0) {
        prev_prime = 2;      /* start from the first prime */
        current_sum = 0;     /* no gaps yet */
    }

    /* Generate primes until we have at least n terms. */
    while (terms_cnt < (size_t)n) {
        uint64_t p_next   = next_prime(prev_prime);          /* next prime */
        uint64_t gap      = p_next - prev_prime;             /* gap between primes */
        seq_t   gap_sq    = (seq_t)gap * (seq_t)gap;          /* square of the gap */
        current_sum      = current_sum + gap_sq;              /* update cumulative sum */

        if (is_prime_big(current_sum)) {                     /* check if sum is prime */
            ensure_terms_cap(terms_cnt + 1);
            terms[terms_cnt] = (seq_t)p_next;                /* store the prime */
            terms_cnt++;
        }

        prev_prime = p_next;                                 /* advance to next prime */
    }

    return terms[n - 1];
}
