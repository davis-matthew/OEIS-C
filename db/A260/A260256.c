/*
 * A260256  Numbers m such that tau(m + 2) = tau(m - 2) where tau(k) =
 *          A000005(k).
 * https://oeis.org/A260256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260256: numbers m such that tau(m+2) = tau(m-2) */
/* Implementation: brute‑force search, memoising the already found terms. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

/* static storage for computed terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t last_checked = 0;

static void ensure_evaluated(idx_t n)
{
    /* initialise storage on first call */
    if (terms == NULL) {
        terms_cap = 128;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        last_checked = 4;               /* first candidate will be 5 */
    }

    /* enlarge capacity if necessary */
    if (terms_cap < (size_t)n) {
        size_t newcap = terms_cap;
        while (newcap < (size_t)n) newcap <<= 1;
        terms = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }

    /* search for new matching numbers until we have n terms */
    idx_t m;
    for (m = last_checked + 1; terms_len < n; ++m) {
        if (m < 5) continue;          /* m-2 would be 0 or negative */
        uint64_t a = (uint64_t)(m - 2);
        uint64_t b = (uint64_t)(m + 2);
        if (divisor_count_u64(a) == divisor_count_u64(b)) {
            terms[terms_len++] = (seq_t)m;
        }
    }
    last_checked = m - 1;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* out of bounds; nothing defined below 1 */
    ensure_evaluated(n);
    return terms[n - 1];
}
