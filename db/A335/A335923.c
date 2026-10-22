/*
 * A335923  A variation on Recamán's sequence (A005132): a(0) = 0, a(n) =
 *          a(n-1) - n if a(n) is nonnegative and not already in the sequence,
 *          otherwise a(n) = a(n-1) + ceiling(n/2).
 * https://oeis.org/A335923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0
   for n > 0:
     if a(n‑1) ≥ n  and (a(n‑1)–n) is not already in the sequence
        a(n) = a(n‑1) – n
     else
        a(n) = a(n‑1) + ceil(n/2)
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t   max_computed = 0;      /* highest index computed so far          */
static seq_t *terms = NULL;          /* dynamic array of sequence values       */
static size_t sz      = 0;           /* current number of stored terms         */
static size_t capacity = 0;          /* allocated capacity of 'terms'          */

/* Ensure storage and initial term are ready. */
static void init(void)
{
    if (!terms) {
        capacity = 1;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!terms) exit(1);            /* abort on allocation failure */
        terms[0] = 0;
        sz = 1;
        max_computed = 0;
    }
}

/* Return a(n) for any n >= 0 (in any order of calls). */
seq_t a(idx_t n)
{
    init();

    if (n > max_computed) {
        /* Extend dynamic array if necessary. */
        if (capacity <= (size_t)n + 1) {
            size_t new_cap = capacity * 2;
            if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            terms = tmp;
            capacity = new_cap;
        }

        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t prev = terms[(size_t)(k - 1)];
            bool cand_valid = (prev >= (seq_t)k);
            seq_t cand = 0;
            if (cand_valid) {
                cand = prev - (seq_t)k;
                /* Check if cand has appeared before. */
                bool found = false;
                for (size_t i = 0; i < sz; ++i) {
                    if (terms[i] == cand) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    terms[sz++] = cand;
                    continue;   /* next k */
                }
            }
            /* Either candidate invalid or already present; use +ceil(k/2). */
            seq_t add = ((seq_t)(k + 1)) / 2;   /* ceil(k/2) */
            terms[sz++] = prev + add;
        }
        max_computed = n;
    }

    return terms[(size_t)n];
}
