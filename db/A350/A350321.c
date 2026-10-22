/*
 * A350321  Totient numbers k such that 14*k is a nontotient.
 * https://oeis.org/A350321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A350321:
   Totient numbers k such that 14*k is a non‑totient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LIMIT 10000000   /* sieve limit to guarantee many terms */

/* Static storage for the sequence.  terms[i] holds the i+1‑st term. */
static seq_t *terms = NULL;
static size_t terms_len = 0;

/* Build the totient sieve up to MAX_LIMIT and collect the terms. */
static void init_terms(void)
{
    /* -------- Euler's totient sieve -------- */
    uint32_t *phi = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!phi) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= (uint32_t)MAX_LIMIT; ++i)
        phi[i] = i;

    for (uint32_t p = 2; p <= (uint32_t)MAX_LIMIT; ++p)
        if (phi[p] == p)              /* prime found */
            for (uint32_t j = p; j <= (uint32_t)MAX_LIMIT; j += p)
                phi[j] -= phi[j] / p;

    /* -------- mark all totient values -------- */
    uint8_t *is_tot = (uint8_t *)malloc((MAX_LIMIT + 1) * sizeof(uint8_t));
    if (!is_tot) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= (uint32_t)MAX_LIMIT; ++i)
        is_tot[i] = 0;
    for (uint32_t i = 1; i <= (uint32_t)MAX_LIMIT; ++i)
        is_tot[phi[i]] = 1;

    /* -------- collect k such that k is a totient
                          and 14*k is not a totient -------- */
    size_t cap = 1024;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    terms_len = 0;

    for (uint32_t k = 1; (uint64_t)k * 14 <= (uint64_t)MAX_LIMIT; ++k) {
        if (is_tot[k] && !is_tot[14 * k]) {
            if (terms_len >= cap) {
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms) exit(EXIT_FAILURE);
            }
            terms[terms_len++] = (seq_t)k;
        }
    }

    /* -------- free temporary data -------- */
    free(phi);
    free(is_tot);
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_terms();
        initialized = true;
    }

    /* n is guaranteed to be >= 1.  Return 0 if beyond precomputed range. */
    return (n - 1 < terms_len) ? terms[n - 1] : (seq_t)0;
}
