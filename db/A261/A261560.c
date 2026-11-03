/*
 * A261560  Semiprimes sp such that (sum of digits of (sp)) + (product of
 *          digits of (sp)) is also semiprime.
 * https://oeis.org/A261560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 5144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9993 (2026-11-03)
 * Helpers:   bigomega_seq, digit_product, digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Semiprimes sp such that (sum of digits of sp) + (product of digits of sp) is also semiprime.
   a(1) = 14, a(2) = 33, a(3) = 38, ...
   This implementation finds terms on demand, caching results so that subsequent calls are fast.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* seq_t holds a term, idx_t is the index type */
    if (n == 0U) return 0U;           /* guard against sub‑offset calls */

    static seq_t *terms = NULL;       /* array of found terms   */
    static size_t terms_len = 0;      /* current number of terms */
    static size_t terms_cap = 0;      /* allocated capacity      */
    static seq_t tested_before = 4U;  /* next candidate to test */

    /* return cached value if already computed */
    if ((size_t)n <= terms_len) {
        return terms[n - 1U];
    }

    /* search for more terms until we have at least n */
    for (seq_t cand = tested_before; ; ++cand) {
        if ((int)bigomega_seq(cand) == 2) {           /* semiprime?          */
            seq_t sum  = digit_sum(cand);
            seq_t prod = digit_product(cand);
            seq_t target = sum + prod;

            if ((int)bigomega_seq(target) == 2) {     /* target also semiprime */
                append_candidate(&terms, &terms_len, &terms_cap, cand);

                if ((size_t)n <= terms_len) {
                    tested_before = cand + 1U;  /* next start point */
                    return terms[n - 1U];
                }
            }
        }
    }
}
