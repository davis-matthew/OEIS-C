/*
 * A397387  Sophie Germain cyclic numbers: numbers k such that k and 2*k+1 are
 *          both cyclic numbers (A003277).
 * https://oeis.org/A397387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 6085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397387: Sophie Germain cyclic numbers.
 * The function returns the n‑th integer k such that both k and
 * 2*k+1 are cyclic (gcd(k,phi(k)) = 1).  It precomputes the first
 * 20000 terms on first call and then indexes the cached array. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */

#define MAX_TERMS        20000     /* enough for n <= 10000 */
#define INITIAL_MAX_K    500000    /* ensures 20000 terms are found */

/* ------------------------------------------------------------------ */
/* Data structures                                                  */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;                /* array of found terms   */
static size_t terms_cnt = 0;               /* number of terms stored */
static uint32_t *phi = NULL;               /* phi values up to 2*MAX_K+1 */

/* ------------------------------------------------------------------ */
/* Helper: linear sieve for Euler's totient                       */
/* ------------------------------------------------------------------ */
static void init_phi(uint32_t limit)
{
    uint32_t max_n = 2u * limit + 1u;
    phi = (uint32_t *)calloc(max_n + 1, sizeof(uint32_t));
    bool *is_comp = (bool *)calloc(max_n + 1, sizeof(bool));
    uint32_t *primes = (uint32_t *)calloc(max_n + 1, sizeof(uint32_t));
    size_t p_cnt = 0;

    phi[0] = 0;
    phi[1] = 1;
    for (uint32_t i = 2; i <= max_n; ++i) {
        if (!is_comp[i]) {
            phi[i] = i - 1;
            primes[p_cnt++] = i;
        }
        for (size_t j = 0; j < p_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t t = (uint64_t)i * p;
            if (t > max_n) break;
            is_comp[t] = true;
            if (i % p == 0) {
                phi[t] = phi[i] * (uint32_t)p;
                break;
            } else {
                phi[t] = phi[i] * (uint32_t)(p - 1);
            }
        }
    }
    free(is_comp);
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a number is cyclic                           */
/* ------------------------------------------------------------------ */
static bool is_cyclic(uint64_t k)
{
    seq_t kk   = (seq_t)k;
    seq_t phik = (seq_t)phi[k];
    if (gcd(kk, phik) != 1) return false;

    uint64_t m = 2 * k + 1;
    seq_t phim = (seq_t)phi[m];
    return gcd((seq_t)m, phim) == 1;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute the sequence                                           */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    init_phi(INITIAL_MAX_K);
    terms      = (seq_t *)calloc(MAX_TERMS, sizeof(seq_t));
    terms_cnt  = 0;

    for (uint32_t k = 1; k <= INITIAL_MAX_K && terms_cnt < MAX_TERMS; ++k) {
        if (is_cyclic(k))
            terms[terms_cnt++] = (seq_t)k;
    }
}

/* ------------------------------------------------------------------ */
/* Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    if (terms == NULL) init_sequence();

    /* we assume n <= terms_cnt because MAX_TERMS is large enough */
    if ((size_t)n > terms_cnt) {
        /* safety: grow the array and keep the old count */
        size_t new_cap = terms_cnt * 2;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) exit(1);
        terms = new_terms;
        /* keep terms_cnt unchanged */
    }

    return terms[(size_t)n - 1];
}
