/*
 * A261792  Primes of the form k*pi(k) - 1, where pi(k) is the number of
 *          primes <= k.
 * https://oeis.org/A261792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of form k*pi(k) - 1, where pi(k) = number of primes
 *            <= k Algorithm: iterate k from 2 upward, maintain pi(k), compute
 *            v = k*pi(k)-1, test primality, and collect the primes until the
 *            requested number of terms is reached. Results are cached for
 *            subsequent calls
 * Range:     u8 n <= 11, u16 n <= 128, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;          /* cached sequence values */
static size_t terms_size = 0;        /* number of values stored */
static size_t terms_cap  = 0;        /* current allocated capacity */

static idx_t next_k = 2;             /* next k to be processed (starting at 2) */
static idx_t pi     = 0;             /* pi(k-1) – number of primes <= next_k-1 */

/* Grow the terms array to at least min_cap elements */
static void grow_terms(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that at least 'needed' terms are computed and cached */
static void ensure_terms(size_t needed)
{
    if (terms_size >= needed) return;

    for (; terms_size < needed; ++next_k)
    {
        /* Update pi(k) */
        if (is_prime_small((seq_t)next_k))
            ++pi;

        /* Compute candidate value v = k*pi(k) - 1 */
        seq_t val = (seq_t)next_k * (seq_t)pi - (seq_t)1;

        /* Check if v is prime and store it if so */
        if (is_prime_big(val))
        {
            grow_terms(terms_size + 1);
            terms[terms_size++] = val;
        }
    }
}

/* Public function required by the benchmark framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* defensive; not used (offset is 1) */
    ensure_terms((size_t)n);
    return terms[n-1];
}
