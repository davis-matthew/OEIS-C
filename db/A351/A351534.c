/*
 * A351534  Odd numbers for which sigma(k) is congruent to 2 modulo 4 and the
 *          3-adic valuation of k is one larger than the 3-adic valuation of
 *          sigma(k).
 * https://oeis.org/A351534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351534
   odd numbers k with σ(k) ≡ 2 (mod 4) and v₃(k) = v₃(σ(k)) + 1
   The function generates terms lazily, memoising them. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;     /* array of found terms          */
    static size_t terms_len  = 0;    /* number of stored terms        */
    static size_t terms_cap  = 0;    /* current capacity              */
    static idx_t  next_k = 1;       /* next odd number to be tested  */

    /* initialisation */
    if (terms == NULL) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        for (idx_t k = next_k; ; k += 2) {   /* only odd k */
            seq_t sigma = sigma_of(k);

            /* σ(k) must be 2 mod 4 */
            if ((sigma & 3) != 2) continue;

            /* 3‑adic valuation of k */
            unsigned vk = 0;
            idx_t tmpk = k;
            while (tmpk % 3 == 0) { tmpk /= 3; ++vk; }

            /* 3‑adic valuation of σ(k) */
            unsigned vs = 0;
            seq_t tmpS = sigma;
            while ((tmpS % 3) == 0) { tmpS /= 3; ++vs; }

            if (vk == vs + 1) {              /* both conditions met */
                /* enlarge storage if needed */
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap * 2;
                    seq_t *new_buf = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!new_buf) exit(EXIT_FAILURE);
                    terms = new_buf;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = k;
                next_k = k + 2;              /* next candidate after this term */
                break;                       /* go on to next missing term     */
            }
        }
    }

    return terms[n - 1];
}
