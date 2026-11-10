/*
 * A397693  Indices of records in A397887.
 * https://oeis.org/A397693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record in A397887, where A397887(1) =
 *            1, A397887(m) = m + A397887(A026238(m)) for m > 1, and
 *            A026238(m) = pi(m) if m is prime, else m - pi(m) - 1. Computed
 *            by a sieve and a linear scan for records, with the limit doubled
 *            until enough records are found
 * Range:     u8 n <= 43, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t *rec_list = NULL;
static size_t rec_cnt = 0;

static void A397693_build(idx_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *pi = (uint32_t *)malloc(((size_t)L + 1) * sizeof(uint32_t));
    seq_t *A = mem_new(seq_t, (size_t)L + 1);
    idx_t *recs = (idx_t *)malloc(((size_t)L + 1) * sizeof(idx_t));
    if (!comp || !pi || !A || !recs) abort();

    comp[0] = 1;
    if (L >= 1) comp[1] = 1;
    for (idx_t i = 2; i * i <= L; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= L; j += i)
                comp[j] = 1;
        }
    }
    uint32_t cnt = 0;
    for (idx_t i = 0; i <= L; ++i) {
        if (!comp[i]) ++cnt;
        pi[i] = cnt;
    }

    size_t rc = 0;
    A[1] = 1;
    recs[rc++] = 1;
    seq_t best = A[1];
    for (idx_t m = 2; m <= L; ++m) {
        idx_t f;
        if (!comp[m]) f = (idx_t)pi[m];
        else f = m - (idx_t)pi[m] - 1;
        A[m] = (seq_t)m + A[f];
        if (A[m] > best) {
            best = A[m];
            recs[rc++] = m;
        }
    }

    free(rec_list);
    rec_list = recs;
    rec_cnt = rc;

    free(comp);
    free(pi);
    mem_free(A);
}

seq_t a(idx_t n)
{
    static idx_t limit = 0;
    if (n < 1) return 0;
    while (rec_cnt < (size_t)n) {
        limit = limit ? limit * 2 : 4096;
        A397693_build(limit);
    }
    return (seq_t)rec_list[n - 1];
}
