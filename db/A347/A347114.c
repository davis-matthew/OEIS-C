/*
 * A347114  Heptagonal pandigitals.
 * https://oeis.org/A347114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n‑th Heptagonal number that is a 10‑digit *
 *            pandigital with no repeated digits. * It is the intersection of
 *            A000566 (heptagonal numbers) and * A171102 (10‑digit pandigital
 *            numbers). The implementation * precomputes all such numbers up
 *            to the largest possible, * which is 9876543210, and...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_unique_pandigital
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_unique_pandigital.h"

/* Static storage for the computed sequence values. */
static seq_t     *terms   = NULL;
static size_t     terms_size = 0;
static size_t     terms_cap  = 0;
static bool       initialized = false;

/* Pre‑compute all terms once. */
static void init_terms(void)
{
    if (initialized) return;
    initialized = true;

    terms_cap = 64;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    terms_size = 0;

    const seq_t LIMIT = (seq_t)9876543210ULL; /* largest 10‑digit pandigital */

    for (idx_t k = 1;; ++k) {
        /* Heptagonal number: H(k) = k * (5k - 3) / 2. */
        seq_t hk = (seq_t)k * (((seq_t)k * (seq_t)5) - (seq_t)3) / (seq_t)2;
        if (hk > LIMIT) break;                /* no more 10‑digit pandigitals */

        if (is_unique_pandigital(hk)) {
            if (terms_size == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) {
                    free(terms);
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
            }
            terms[terms_size++] = hk;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset 1, return 0 safely */

    init_terms();

    if ((size_t)n - 1 >= terms_size) return (seq_t)0;  /* no such term */

    return terms[(size_t)n - 1];
}
