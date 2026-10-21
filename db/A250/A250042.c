/*
 * A250042  Numbers n such that m = floor(n/9) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is a term such that m = floor(a(n)/9) is coprime with a(n)
 *            and if m>0 then m is itself a term of the sequence
 * Range:     u8 n <= 58, u16 n <= 3212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *terms = NULL;      /* array of sequence terms (sorted ascending) */
static size_t terms_len = 0;     /* number of terms currently stored   */
static size_t terms_cap = 0;     /* current allocated capacity         */
static idx_t  last_checked = 0;  /* largest candidate examined         */

/* Binary search: true iff x is already a term. */
static bool is_in_terms(seq_t x)
{
    size_t lo = 0, hi = terms_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (terms[mid] == x) return true;
        if (terms[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Grow the terms array if necessary. */
static void ensure_terms_capacity(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Main sequence generator with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; behaviour for n<1 does not matter */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        idx_t candidate_idx = ++last_checked;          /* next candidate to test   */
        seq_t cand = (seq_t)candidate_idx;             /* candidate value         */
        seq_t m   = cand / 9;                          /* floor(cand / 9)         */

        /* Check coprimality. gcd(cand,0) equals cand, so only cand=1 passes. */
        if (gcd(cand, m) == 1) {
            /* Accept cand if m==0 or m is already a term. */
            if (m == 0 || is_in_terms(m)) {
                ensure_terms_capacity();
                terms[terms_len++] = cand;
            }
        }
    }

    return terms[n - 1];
}
