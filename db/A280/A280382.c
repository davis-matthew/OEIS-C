/*
 * A280382  Numbers k such that k-1 has the same number of prime factors
 *          counted with multiplicity as k+1.
 * https://oeis.org/A280382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that Ω(k-1) = Ω(k+1), where Ω counts prime
   factors with multiplicity (A001222).  The function keeps a static
   dynamically‑grown array of discovered terms so that calls in any
   order remain fast. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset for OEIS is 1 */
        return 0;

    static seq_t *terms = NULL;     /* array of found terms  */
    static size_t terms_len = 0;    /* number of terms stored   */
    static size_t terms_cap = 0;    /* capacity of the array    */
    static idx_t last_checked = 3;  /* last k examined (next is 4) */

    while (terms_len < n) {
        idx_t k = ++last_checked;                 /* next candidate   */
        seq_t om1 = count_prime_factors((seq_t)(k - 1));
        seq_t op1 = count_prime_factors((seq_t)(k + 1));

        if (om1 == op1) {                         /* satisfy condition */
            if (terms_len == terms_cap) {         /* grow array       */
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp)                         /* allocation failure */
                    return 0;
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;        /* store the term   */
        }
    }

    return terms[n - 1];                          /* a(1) is terms[0] */
}
