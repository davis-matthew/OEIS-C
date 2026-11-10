/*
 * A368402  Numbers k such that k and k+1 are both in A268375.
 * https://oeis.org/A368402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = n-th k such that k and k+1 are both in A268375
 *            (prime exponents of k have pairwise disjoint binary
 *            representations)
 * Range:     u8 n <= 47, u16 n <= 1409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9839 (2026-11-10)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static bool A268375_prop(uint64_t n)
{
    if (n <= 1) return true;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t used = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (used & ex[i]) return false;
        used |= ex[i];
    }
    return true;
}

static seq_t *A368402_terms = NULL;
static size_t A368402_len = 0;
static size_t A368402_cap = 0;
static uint64_t A368402_k = 1;          /* next candidate k */
static bool A368402_pk = true;          /* property of A368402_k */

seq_t a(idx_t n)
{
    while (A368402_len < (size_t)n) {
        bool pn = A268375_prop(A368402_k + 1);
        if (A368402_pk && pn) {
            if (A368402_len == A368402_cap) {
                size_t nc = A368402_cap ? A368402_cap * 2 : 256;
                seq_t *t = mem_resize(seq_t, A368402_terms, nc);
                if (!t) abort();
                A368402_terms = t;
                A368402_cap = nc;
            }
            A368402_terms[A368402_len++] = (seq_t)A368402_k;
        }
        A368402_pk = pn;
        ++A368402_k;
    }
    return A368402_terms[n - 1];
}
