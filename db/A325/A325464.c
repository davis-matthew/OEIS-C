/*
 * A325464  a(n) is the least number not 2*a(m) or 3*a(m)-2 for any m < n.
 * https://oeis.org/A325464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   forb_contains, insert_sorted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325464
   a(n) = the least integer not equal to 2*a(m) or 3*a(m)-2 for any m < n.
   The sequence is built greedily: starting with a(1)=1, each next term
   is the smallest positive integer that hasn't appeared yet and isn't
   forbidden by previous terms.  */
#include <stdlib.h>
#include <string.h>
#include "../utils/forb_contains.h"
#include "../utils/insert_sorted.h"

/* static arrays that hold the computed sequence and the forbidden values
   We keep them sorted so we can binary‑search (forb_contains) and
   insert (insert_sorted).  */
static seq_t *seq      = NULL; /* a(1), a(2), ... */
static size_t seq_len  = 0;    /* number of terms stored   */
static size_t seq_cap  = 0;    /* allocated capacity       */

static seq_t *forb     = NULL; /* forbidden values      */
static size_t forb_len = 0;    /* number of forbidden values */
static size_t forb_cap = 0;    /* allocated capacity   */

/* The requested function.  It keeps the two static arrays between calls
   so that any order of invocation works.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used by OEIS, but harmless */

    /* Initialise static storage on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        forb_cap = 32;
        forb     = (seq_t *)malloc(forb_cap * sizeof(seq_t));

        /* a(1) = 1 */
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* forbidden values from a(1) */
        insert_sorted(&forb, &forb_len, &forb_cap, (seq_t)1);   /* 3*1-2 */
        insert_sorted(&forb, &forb_len, &forb_cap, (seq_t)2);   /* 2*1   */
    }

    /* Generate terms up to n */
    while (seq_len < (size_t)n) {
        /* start searching just after the current maximum term */
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;

        /* skip all forbidden values */
        while (forb_contains(forb, forb_len, candidate))
            candidate = candidate + (seq_t)1;

        /* append the new term */
        if (seq_len >= seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = candidate;

        /* add new forbidden values coming from this term */
        seq_t f1 = candidate * (seq_t)2;      /* 2*candidate      */
        seq_t f2 = candidate * (seq_t)3 - (seq_t)2; /* 3*candidate-2 */
        insert_sorted(&forb, &forb_len, &forb_cap, f1);
        insert_sorted(&forb, &forb_len, &forb_cap, f2);
    }

    return seq[n - 1];
}
