/*
 * A295587  Numbers k such that Bernoulli number B_{k} has denominator 13530.
 * https://oeis.org/A295587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th even integer k such that the Bernoulli number
 *            B_k has denominator 13530. Implementation: pre‑compute all
 *            admissible k up to a fixed maximum using a sieve that marks
 *            every multiple of (p-1) for all primes p not in {2,3,5,11,41}.
 *            All remaining multiples of 40 are the desired terms
 * Range:     u8 n <= 1, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* maximum k that is searched for; large enough for n up to ~20000 */
#define MAX_K 20000000UL

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static bool initialized = false;

/* helper to grow the terms array */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 16384;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* ---------- initialization routine ---------- */
static void init(void)
{
    size_t i, j;
    unsigned char *is_prime = NULL;
    unsigned char *forbidden = NULL;

    /* allocate prime sieve mask */
    is_prime = (unsigned char *)malloc(MAX_K + 2);
    if (!is_prime) exit(1);
    memset(is_prime, 1, MAX_K + 2);
    is_prime[0] = is_prime[1] = 0;

    /* simple Eratosthenes sieve up to MAX_K + 1 */
    for (i = 2; i * i <= MAX_K + 1; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= MAX_K + 1; j += i)
                is_prime[j] = 0;
        }
    }

    /* allocate array that marks forbidden k */
    forbidden = (unsigned char *)malloc(MAX_K + 1);
    if (!forbidden) exit(1);
    memset(forbidden, 0, MAX_K + 1);

    /* mark forbidden multiples of (p-1) for all primes p
       except p in {2,3,5,11,41}. */
    for (i = 2; i <= MAX_K + 1; ++i) {
        if (!is_prime[i]) continue;
        if (i == 2 || i == 3 || i == 5 || i == 11 || i == 41) continue;
        size_t d = i - 1;                 /* (p-1) */
        for (j = d; j <= MAX_K; j += d)
            forbidden[j] = 1;
    }

    /* free sieve memory */
    free(is_prime);

    /* build the list of admissible k (multiples of 40) */
    for (i = 40; i <= MAX_K; i += 40) {
        if (!forbidden[i]) {
            grow_terms();
            terms[terms_len++] = (seq_t)i;
        }
    }

    free(forbidden);
    initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n == 0) return (seq_t)0;           /* undefined, return 0 */
    size_t idx = (size_t)(n - 1);
    if (idx >= terms_len) return (seq_t)0; /* out of precomputed range */
    return terms[idx];
}
