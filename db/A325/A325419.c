/*
 * A325419  a(n) is the least number not 2*a(m)+1 or 3*a(m) for any m < n.
 * https://oeis.org/A325419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest integer not equal to 2*a(m)+1 or 3*a(m) for
 *            any m < n. (OEIS A325419)
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/grow_uint8.h"

/* Static state for memoisation */
static seq_t *SEQ    = NULL;  /* computed sequence values */
static size_t    seq_cap = 0; /* allocated capacity of SEQ     */
static size_t    seq_len = 0; /* number of values stored      */

static uint8_t *USED = NULL; /* array marking values that have appeared as a(m) */
static size_t   used_cap = 0;/* allocated capacity of USED */
static uint8_t *FORB = NULL; /* array marking values that are forbidden (2*a+1 or 3*a) */
static size_t   forb_cap = 0;/* allocated capacity of FORB */

static idx_t next_candidate = 1; /* smallest integer to start searching from */

/* Ensure a seq_t array is at least 'need' elements, new entries default‑constructed */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* out of offset */

    /* Return cached value if already computed */
    if (n <= seq_len) return SEQ[n-1];

    /* Generate terms up to n */
    while (seq_len < n) {

        /* Find the next admissible candidate */
        idx_t cand = next_candidate;
        grow_uint8(&USED,   &used_cap,   cand + 1);
        grow_uint8(&FORB,   &forb_cap,   cand + 1);

        while (USED[cand] || FORB[cand]) {
            ++cand;
            grow_uint8(&USED, &used_cap, cand + 1);
            grow_uint8(&FORB, &forb_cap, cand + 1);
        }

        /* Record the term */
        seq_t val = (seq_t)cand;
        grow_seq(&SEQ, &seq_cap, seq_len + 1);
        SEQ[seq_len++] = val;

        /* Mark this value as used */
        USED[cand] = 1;

        /* Mark future forbidden values */
        idx_t f1 = 3 * cand;      /* 3*a(m) */
        idx_t f2 = 2 * cand + 1;  /* 2*a(m)+1 */

        grow_uint8(&FORB, &forb_cap, f1 + 1);
        grow_uint8(&FORB, &forb_cap, f2 + 1);
        FORB[f1] = 1;
        FORB[f2] = 1;

        /* Next search begins just after this value */
        next_candidate = cand + 1;
    }

    return SEQ[n-1];
}
