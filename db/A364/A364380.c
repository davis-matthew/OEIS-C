/*
 * A364380  Numbers k such that k and k+1 are both greedy Jacobsthal-Niven
 *          numbers (A364379).
 * https://oeis.org/A364380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k (k≥1) such that both k and k+1 are
 *            greedy Jacobsthal‑Niven numbers
 * Range:     u8 n <= 40, u16 n <= 1044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_greedy_jacobsthal_niven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: on‑demand search with memoisation; O(n log k) time. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_greedy_jacobsthal_niven.h"

/* seq_t and idx_t are supplied by the build; do not redeclare them. */

/* ---------------------------------- */
/* Main sequence function */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n ≤ 0, but harmless */

    /* Static cache of the found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* number of cached terms */
    static size_t terms_cap = 0;   /* allocated capacity */
    static seq_t next_candidate = 1;  /* next integer to test */

    while (terms_len < (size_t)n) {
        seq_t curr = next_candidate;
        if (is_greedy_jacobsthal_niven(curr) &&
            is_greedy_jacobsthal_niven(curr + 1)) {
            /* Store the term. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* Out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = curr;
        }
        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}
