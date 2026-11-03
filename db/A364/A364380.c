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
 * Validated: standard, pass, 76 terms through n = 9661 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: on‑demand search with memoisation; O(n log k) time. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether N is a greedy Jacobsthal‑Niven number. */
static bool is_greedy_jacobsthal_niven(seq_t N)
{
    static seq_t *jac = NULL;      /* Jacobsthal numbers array */
    static size_t jac_len = 0;     /* number of generated Jacobsthal numbers */
    static size_t jac_cap = 0;     /* allocated capacity */

    /* Grow the Jacobsthal array so that the last element exceeds N. */
    while (jac_len == 0 || jac[jac_len - 1] <= N) {
        if (jac_len == jac_cap) {
            size_t new_cap = jac_cap ? jac_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, jac, new_cap);
            if (!tmp) abort();      /* Out of memory – terminate. */
            jac = tmp;
            jac_cap = new_cap;
        }

        /* The first two Jacobsthal numbers are 1, 1. */
        if (jac_len < 2) {
            jac[jac_len++] = 1;
            continue;
        }

        /* J(n) = J(n‑1) + 2*J(n‑2). */
        seq_t next = jac[jac_len - 1] + (jac[jac_len - 2] << 1);
        jac[jac_len++] = next;
    }

    /* Greedy representation of N using the precomputed Jacobsthal numbers. */
    seq_t rem = N;
    seq_t sum = 0;                 /* digit sum = number of subtractions */
    for (size_t i = jac_len; i-- > 0;) {
        while (jac[i] <= rem) {
            rem -= jac[i];
            ++sum;
        }
    }

    /* N is greedy Jacobsthal‑Niven iff the remainder is zero and N % sum == 0. */
    return (rem == 0 && sum != 0 && N % sum == 0);
}

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
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
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
