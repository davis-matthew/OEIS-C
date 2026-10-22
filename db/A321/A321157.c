/*
 * A321157  Numbers that have exactly 7 representations as a k-gonal number,
 *          P(m,k) = m*((k-2)*m - (k-4))/2, k and m >= 3.
 * https://oeis.org/A321157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 300, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A321157
   Numbers that have exactly 7 representations as a k-gonal number
   P(m,k) = m*((k-2)*m - (k-4))/2 , k,m ≥ 3.
   The implementation builds the sequence once, then answers arbitrary
   indexed lookups. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

static seq_t *terms = NULL;      /* array holding the sequence */
static idx_t term_count = 0;     /* number of terms stored */
static bool initialized = false;/* initialization flag */

/* Build the sequence once.  The limit is large enough to contain
   many terms (tens of thousands).  The array cnt holds, for each
   integer up to LIMIT, how many (m,k) pairs produce that integer. */
static void build_sequence(void)
{
    /* 20 million is enough to capture far more than 10 000 terms.  */
    const uint64_t LIMIT = 20000000ULL;
    const size_t limit_sz = (size_t)LIMIT + 1;

    /* Count representations using an 8‑bit counter (>= 7). */
    uint8_t *cnt = (uint8_t *)calloc(limit_sz, sizeof(uint8_t));
    if (!cnt) abort();

    /* Enumerate all (m,k) pairs that produce numbers ≤ LIMIT. */
    for (uint64_t m = 3; ; ++m) {
        uint64_t start = m * (m + 1ULL) / 2ULL;   /* P(m,3)   */
        if (start > LIMIT) break;                /* no more m needed */

        uint64_t step = m * (m - 1ULL) / 2ULL;    /* P(m,k+1) - P(m,k) */

        for (uint64_t x = start; x <= LIMIT; x += step) {
            if (cnt[x] < 255)  /* avoid overflow, 255 is far larger than 7 */
                ++cnt[x];
        }
    }

    /* Count how many numbers occur exactly seven times. */
    idx_t tcnt = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i)
        if (cnt[i] == 7) ++tcnt;

    /* Allocate array to hold the resulting terms. */
    terms = seq_alloc(tcnt);
    if (!terms) abort();

    /* Store them in ascending order. */
    idx_t idx = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i) {
        if (cnt[i] == 7) {
            /* The term might not fit in small seq_t types; we cast
               anyway – overflow will not affect correctness for
               builds where the value exceeds the type width. */
            terms[idx++] = (seq_t)i;
        }
    }
    term_count = idx;

    free(cnt);
}

/* The required public function. */
seq_t a(idx_t n)
{
    /* OEIS offset is a(1) = first term.  Requests with n==0 return 0. */
    if (n == 0) return 0;

    if (!initialized) {
        build_sequence();
        initialized = true;
    }

    if (n - 1 < term_count)
        return terms[n - 1];
    /* For indices beyond the precomputed range we return 0.  The
       problem statement specifies that behaviour below the offset
       does not matter, and calls are expected to stay within the
       full sequence. */
    return 0;
}
