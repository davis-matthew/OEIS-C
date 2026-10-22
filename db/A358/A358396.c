/*
 * A358396  Even numbers k such that sigma(k) + sigma(k+2) < 2*sigma(k+1);
 *          even terms in A053229.
 * https://oeis.org/A358396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 529, u32 n <= 9226, u64 n <= 9226,
 *            u128 n <= 9226, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358396: a(n) = nth even k such that σ(k)+σ(k+2) < 2σ(k+1). */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build. */
/* The only used utility is sigma_of: seq_t sigma_of(idx_t n). */
#include "../utils/sigma_of.h"

static seq_t *terms   = NULL;   /* cached sequence values */
static idx_t  terms_len = 0;    /* number of terms stored */
static idx_t  terms_cap = 0;    /* allocated capacity */
static idx_t  cur_k     = 104;  /* next even candidate to test */

/* Ensure the terms array has space for at least 'needed' elements. */
static void grow_terms(idx_t needed)
{
    if (terms_cap >= needed) return;
    idx_t new_cap = terms_cap ? (terms_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(terms, sizeof(seq_t) * new_cap);
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the nth term (n ≥ 1).  The function is safe for calls in any
 * order and memoises previously found terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0, but avoid UB */
    while (terms_len < n) {
        /* σ(k), σ(k+1), σ(k+2) using the provided helper. */
        seq_t s0 = sigma_of(cur_k);
        seq_t s1 = sigma_of(cur_k + 1);
        seq_t s2 = sigma_of(cur_k + 2);

        /* Check σ(k)+σ(k+2) < 2σ(k+1). */
        seq_t lhs = s0 + s2;
        seq_t rhs = s1 + s1;
        if (lhs < rhs) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = cur_k;
        }
        cur_k += 2;  /* test only even numbers */
    }
    return terms[n - 1];
}
