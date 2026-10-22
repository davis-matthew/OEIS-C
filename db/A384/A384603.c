/*
 * A384603  Primes preceded and followed by gaps whose quotient (value greater
 *          or equal to 1) is less than 2.
 * https://oeis.org/A384603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2457, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A384603: primes p such that
   Max(p‑prev, next‑p) / Min(p‑prev, next‑p) < 2.
   The function returns the n‑th such prime (1‑based). */
/* The implementation pre‑computes all qualifying primes up to a fixed
   limit that guarantees coverage for n ≤ 10000.  It works for any
   seq_t and idx_t defined externally. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *qualifying = NULL;   /* array of qualifying primes */
static size_t qual_len   = 0;       /* number of selected primes */
static size_t qual_cap   = 0;       /* allocated capacity */
static bool initialized  = false;  /* sieve built? */

/* Build the sieve, collect primes, and filter the qualifying ones. */
static void init_all(void)
{
    if (initialized)
        return;
    initialized = true;

    /* --- Sieve of Eratosthenes up to LIM --- */
    const idx_t LIM = 5000000ULL;   /* 5 million is enough for 10 000 terms */
    bool *is_prime = (bool*)malloc((size_t)LIM + 1);
    if (!is_prime) {
        qual_len = 0;
        return;
    }
    for (size_t i = 0; i <= (size_t)LIM; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= (size_t)LIM; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)LIM; j += i)
                is_prime[j] = false;
        }
    }

    /* --- Collect all primes into a temporary array --- */
    size_t pcnt = 0;
    for (size_t i = 2; i <= (size_t)LIM; ++i)
        if (is_prime[i])
            ++pcnt;
    uint32_t *pr = (uint32_t*)malloc(pcnt * sizeof(uint32_t));
    if (!pr) {
        free(is_prime);
        qual_len = 0;
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)LIM; ++i)
        if (is_prime[i])
            pr[idx++] = (uint32_t)i;

    /* --- Filter qualifying primes --- */
    qual_cap = pcnt / 3 + 16;                 /* initial capacity estimate */
    qualifying = (seq_t*)malloc(qual_cap * sizeof(seq_t));
    if (!qualifying) {
        free(is_prime);
        free(pr);
        qual_len = 0;
        return;
    }
    qual_len = 0;
    for (size_t k = 1; k + 1 < pcnt; ++k) {
        uint32_t g1 = pr[k]   - pr[k - 1];
        uint32_t g2 = pr[k + 1] - pr[k];
        uint32_t mn = g1 < g2 ? g1 : g2;
        uint32_t mx = g1 > g2 ? g1 : g2;
        if (mx < 2 * mn) {          /* quotient < 2 */
            if (qual_len == qual_cap) {
                qual_cap <<= 1;
                qualifying = (seq_t*)realloc(qualifying,
                                      qual_cap * sizeof(seq_t));
                if (!qualifying) break;
            }
            qualifying[qual_len++] = (seq_t)pr[k];
        }
    }

    free(is_prime);
    free(pr);
}

/* Public routine required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0 */
    init_all();
    if ((size_t)n <= qual_len)
        return qualifying[n - 1];
    return (seq_t)0;              /* placeholder if n exceeds precomputed range */
}
