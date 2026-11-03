/*
 * A383288  Numbers k for which A276075(k) is a multiple of 4, where A276075
 *          is fully additive with a(p) = PrimePi(p)!.
 * https://oeis.org/A383288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k such that * A276075(k) is a
 *            multiple of 4. * * For a number k, let v2(k), v3(k), v5(k) be
 *            the exponents of 2,3,5 in its * prime factorisation. Then *
 *            A276075(k) ≡ v2(k) + 2·(v3(k)+v5(k)) (mod 4). * Hence k belongs
 *            to the set iff * ( v2(k) + 2·(v3(k)+v5(k)) ) ≡ 0...
 * Range:     u8 n <= 103, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 87 terms through n = 9796 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a given integer n satisfies the 4‑divisibility condition
 * for A276075.  n is an idx_t (unsigned 64‑bit).  The function returns
 * true if the number belongs to the sequence.  Only the exponents of
 * 2, 3, and 5 are relevant. */
static bool
valid_member(idx_t n)
{
    idx_t e2 = 0, e3 = 0, e5 = 0, t = n;

    /* exponent of 2 */
    while ((t & 1) == 0) {
        ++e2;
        t >>= 1;
    }
    /* exponent of 3 */
    while (t % 3 == 0) {
        ++e3;
        t /= 3;
    }
    /* exponent of 5 */
    while (t % 5 == 0) {
        ++e5;
        t /= 5;
    }
    /* Check the congruence (e2 + 2*(e3+e5)) ≡ 0 (mod 4) */
    return (((e2 + 2 * (e3 + e5)) & 3) == 0);
}

/* Memoised table of already computed terms. */
static seq_t *terms = NULL;      /* array of sequence values */
static size_t terms_len = 0;     /* number of stored terms */
static size_t terms_cap = 0;     /* allocated capacity of terms */
static idx_t next_candidate = 1; /* next integer to test for inclusion */

/* Append a new term to the table, resizing if necessary. */
static void
append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
        seq_t *new_ptr = mem_resize(seq_t, terms, new_cap);
        if (!new_ptr) abort();            /* out of memory */
        terms = new_ptr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but guard against */
    /* initialise table with the first term 1 */
    if (terms_len == 0) {
        append_term((seq_t)1);
        next_candidate = 2;
    }
    /* extend table until we reach the requested index n */
    while (terms_len < n) {
        /* find the next integer that satisfies the condition */
        while (!valid_member(next_candidate))
            ++next_candidate;
        /* append it to the table */
        append_term((seq_t)next_candidate);
        ++next_candidate;
    }
    return terms[n - 1];
}
