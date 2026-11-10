/*
 * A272054  Complement of A262775.
 * https://oeis.org/A272054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m>2 rejected by the sieve of A262775 (terms
 *            0,2; m is kept iff m mod t(j) != t(j-1) for all earlier terms
 *            t); computed by a sieve/search
 * Range:     u8 n <= 211, u16 n <= 9187, u32 n <= 9187, u64 n <= 9187,
 *            u128 n <= 9187, big any n
 * Validated: standard, pass, 82 terms through n = 8625 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A_terms = NULL;
static idx_t A_tcap = 0, A_tlen = 0;
static seq_t *A_rej = NULL;
static idx_t A_rcap = 0, A_rlen = 0;
static seq_t A_next = 3;

static void A_push_term(seq_t v)
{
    if (A_tlen >= A_tcap) {
        idx_t nc = A_tcap ? A_tcap * 2 : 16;
        seq_t *p = mem_resize(seq_t, A_terms, nc);
        if (!p) abort();
        A_terms = p; A_tcap = nc;
    }
    A_terms[A_tlen++] = v;
}

static void A_push_rej(seq_t v)
{
    if (A_rlen >= A_rcap) {
        idx_t nc = A_rcap ? A_rcap * 2 : 64;
        seq_t *p = mem_resize(seq_t, A_rej, nc);
        if (!p) abort();
        A_rej = p; A_rcap = nc;
    }
    A_rej[A_rlen++] = v;
}

seq_t a(idx_t n)
{
    if (A_tlen == 0) {
        A_push_term((seq_t)0);
        A_push_term((seq_t)2);
    }
    while (A_rlen < n) {
        seq_t m = A_next;
        bool ok = true;
        for (idx_t k = 1; k < A_tlen; ++k) {
            if (m % A_terms[k] == A_terms[k - 1]) { ok = false; break; }
        }
        if (ok) A_push_term(m);
        else A_push_rej(m);
        A_next = m + 1;
    }
    return A_rej[n - 1];
}
