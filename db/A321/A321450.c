/*
 * A321450  Numbers that are sums of consecutive pentagonal pyramidal numbers
 *          (A002411).
 * https://oeis.org/A321450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are sums of consecutive pentagonal pyramidal numbers (A002411).
   The sequence a(n) lists these sums in increasing order, starting with a(1)=0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_free.h"

/* Utility compare function for qsort. */
static int compare_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Allocate an array of seq_t and return a pointer. */
static seq_t *seq_alloc(size_t cap)
{
    return (seq_t *)malloc(sizeof(seq_t) * cap);
}

/* Compute all distinct sums of consecutive pentagonal pyramidal numbers
   using the first m terms.  The result is stored in *out (sorted, unique),
   and the number of elements is stored in *out_len. */
static void compute_terms(idx_t m, seq_t **out, size_t *out_len)
{
    /* Prefix sums of pentagonal pyramidal numbers: PPN(k) = k^2(k+1)/2. */
    size_t pref_len = (size_t)(m + 1);
    seq_t *pref = seq_alloc(pref_len);
    pref[0] = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t kk = (seq_t)k;
        seq_t term = kk * kk * (kk + 1) / 2;   /* PPN(k) */
        pref[(size_t)k] = pref[(size_t)(k - 1)] + term;
    }

    /* Generate all sums of consecutive PPNs (as differences of prefix sums). */
    size_t raw_len = (size_t)m * (m + 1) / 2;
    seq_t *raw = seq_alloc(raw_len);
    size_t rpos = 0;
    for (idx_t i = 0; i < m; ++i) {
        for (idx_t j = i + 1; j <= m; ++j) {
            raw[rpos++] = pref[(size_t)j] - pref[(size_t)i];
        }
    }
    seq_free(pref);

    /* Sort and deduplicate to get the final sequence (plus the zero term). */
    qsort(raw, raw_len, sizeof(seq_t), compare_seq_t);

    seq_t *terms = seq_alloc(raw_len + 1); /* +1 for the zero term */
    size_t tpos = 0;
    terms[tpos++] = 0; /* include 0 as the first term */
    for (size_t i = 0; i < raw_len; ++i) {
        if (i == 0 || raw[i] != raw[i - 1]) {
            terms[tpos++] = raw[i];
        }
    }
    seq_free(raw);

    *out = terms;
    *out_len = tpos;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Static state for memoization. */
    static bool initialized = false;
    static idx_t current_m = 0;
    static seq_t *terms = NULL;
    static size_t terms_len = 0;

    if (!initialized) {
        /* Initial range of terms. */
        const idx_t INIT_M = 200;
        compute_terms(INIT_M, &terms, &terms_len);
        current_m = INIT_M;
        initialized = true;
    }

    /* If the requested index is beyond the current table, enlarge it. */
    while ((idx_t)(n - 1) >= terms_len) {
        idx_t new_m = current_m * 2 + 100;   /* growth heuristic */
        seq_t *new_terms = NULL;
        size_t new_len = 0;
        compute_terms(new_m, &new_terms, &new_len);
        seq_free(terms);                     /* release old memory */
        terms = new_terms;
        terms_len = new_len;
        current_m = new_m;
    }

    return terms[n - 1];
}
