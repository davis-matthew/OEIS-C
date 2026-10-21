/*
 * A257417  Values of n such that there are exactly 10 solutions to x^2 - y^2
 *          = n with x > y >= 0.
 * https://oeis.org/A257417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A257417: numbers n with exactly 10 solutions to
    x^2 - y^2 = n (x > y ≥ 0).  The implementation sieves up to a
    fixed limit, counts factor pairs of equal parity, and stores the
    resulting n's in a static array indexed from 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static seq_t *terms  = NULL;
    static size_t term_cnt = 0;
    static size_t terms_cap = 0;

    if (!initialized) {
        /* Upper bound to generate enough terms for n up to about 10 000 */
        const idx_t LIMIT = 20000000ULL;          /* 2 x 10^7 */

        /* Count solutions for each n: number of factor pairs (d,k)
           with d*k = n and d, k of the same parity.  Use 16‑bit
           counters because we only care up to 10. */
        uint16_t *cnt = (uint16_t *)calloc((size_t)LIMIT + 1, sizeof(uint16_t));
        if (!cnt) {
            return (seq_t)0;
        }

        for (idx_t d = 1; d * d <= LIMIT; ++d) {
            for (idx_t k = d; d * k <= LIMIT; ++k) {
                if (((d & 1) == (k & 1))) {
                    ++cnt[d * k];
                }
            }
        }

        /* Allocate space for the terms */
        terms_cap = 16384;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) {
            free(cnt);
            return (seq_t)0;
        }

        term_cnt = 0;
        for (idx_t i = 1; i <= LIMIT; ++i) {
            if (cnt[i] == 10) {
                if (term_cnt == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(cnt);
                        free(terms);
                        return (seq_t)0;
                    }
                    terms = tmp;
                }
                terms[term_cnt++] = (seq_t)i;
            }
        }

        free(cnt);
        initialized = true;
    }

    /* Return the required term (1‑indexed).  For out‑of‑range n return 0. */
    if (n == 0 || n > term_cnt) {
        return (seq_t)0;
    }
    return terms[n - 1];
}
