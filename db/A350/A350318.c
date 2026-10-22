/*
 * A350318  Totient numbers k such that 7*k is a nontotient.
 * https://oeis.org/A350318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A350318: 
   Totient numbers k for which 7*k is not a totient.
   We sieve the Euler totient function up to a fixed bound (2 000 000),
   mark which values appear as φ(n), and collect all k with the said
   property. The collected list is stored in the array `terms`, and
   subsequent calls to `a(n)` simply index into this array. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Maximum k to consider – enough to generate all terms needed for the
   test cases (≈2 000 000). Adjust if more terms are required. */
#define MAX_K 2000000U

/* Forward declaration of the public sequence function */
seq_t a(idx_t n);

/* Internal state */
static bool          initialized = false;
static size_t        max_terms   = 0;
static seq_t        *terms       = NULL;

/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    /* Total range needed for φ and 7·k */
    const uint64_t max_val = 7U * MAX_K;

    /* Allocate arrays for φ and existence table */
    uint32_t *phi = (uint32_t *)malloc((max_val + 1U) * sizeof(uint32_t));
    uint8_t  *exists = (uint8_t *)malloc((max_val + 1U) * sizeof(uint8_t));
    if (!phi || !exists)
        abort();  /* OOM */

    /* Initialise φ[i] = i and existence flags */
    for (uint64_t i = 0; i <= max_val; ++i) {
        phi[i]   = (uint32_t)i;
        exists[i]= 0;
    }

    /* Sieve for Euler's totient function */
    for (uint64_t i = 2; i <= max_val; ++i) {
        if (phi[i] == (uint32_t)i) {          /* i is prime */
            for (uint64_t j = i; j <= max_val; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }

    /* Record values that occur as a totient */
    for (uint64_t i = 1; i <= max_val; ++i) {
        exists[phi[i]] = 1;
    }

    /* Collect k such that k is a totient and 7*k is not a totient */
    size_t cap = MAX_K / 5;          /* starting capacity estimate */
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms)
        abort();
    max_terms = 0;

    for (uint64_t k = 1; k <= MAX_K; ++k) {
        if (exists[k] && !exists[7 * k]) {
            if (max_terms == cap) {
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms)
                    abort();
            }
            terms[max_terms++] = (seq_t)k;
        }
    }

    /* Clean up */
    free(phi);
    free(exists);
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    if (n == 0 || n > (idx_t)max_terms)
        return (seq_t)0;  /* undefined for out‑of‑range indices */
    return terms[(size_t)(n - 1)];
}
