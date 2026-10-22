/*
 * A333100  Even numbers k such that both k and k + 2 are nontotients
 *          (A005277).
 * https://oeis.org/A333100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find even numbers k such that both k and k+2 are nontotients (values not in A005277). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static const idx_t LIMIT = 2000000ULL;          /* sieve bound – enough for n≈10 000 */
static bool   initialized = false;
static uint32_t *phi     = NULL;                /* φ(0..LIMIT+2)  */
static unsigned char *is_totient = NULL;      /* is_totient[x] = 1 iff x is a totient */
static seq_t *terms   = NULL;                  /* list of qualifying k */
static size_t terms_count  = 0;
static size_t terms_cap    = 0;

/* Helper to grow the terms array. */
static void grow_terms(void)
{
    size_t new_cap = (terms_cap == 0) ? 4096 : terms_cap * 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory – terminate */
    terms = tmp;
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* ---------- 1. Allocate and compute φ for 0..LIMIT+2 ---------- */
        size_t size = LIMIT + 3;
        phi = (uint32_t *)malloc(size * sizeof(uint32_t));
        if (!phi) abort();
        is_totient = (unsigned char *)calloc(size, sizeof(unsigned char));
        if (!is_totient) abort();

        for (idx_t i = 0; i < size; ++i) phi[i] = (uint32_t)i;

        /* Sieve to compute φ. */
        for (idx_t i = 2; i < size; ++i) {
            if (phi[i] == (uint32_t)i) {                /* i is prime */
                for (idx_t j = i; j < size; j += i)
                    phi[j] -= phi[j] / (uint32_t)i;
            }
        }

        /* Mark all totient values. */
        for (idx_t i = 1; i < size; ++i) {
            uint32_t val = phi[i];
            if (val < size)
                is_totient[val] = 1;
        }

        /* ---------- 2. Collect qualifying even k ---------- */
        terms_cap = 65536;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms_count = 0;

        for (idx_t k = 2; k + 2 < size; k += 2) {
            if (!is_totient[k] && !is_totient[k + 2]) {
                if (terms_count == terms_cap) grow_terms();
                terms[terms_count++] = (seq_t)k;
            }
        }

        initialized = true;
    }

    /* If n is larger than we have precomputed, refuse to compute further
       – the problem guarantees n up to about 10 000 and LIMIT is large
       enough to cover that. */
    if (n == 0 || n > terms_count) return 0;
    return terms[n - 1];
}
