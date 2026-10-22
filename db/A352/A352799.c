/*
 * A352799  Inventory sequence of binary weights.
 * https://oeis.org/A352799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1055, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_counts_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Inventory sequence of binary weights (OEIS A352799)
   Implements the round‑by‑round inventory of binary weights as
   described: start with a(0)=0, then at each round record
   counts of terms having each binary weight, adding the newly
   recorded count as a term and updating the counts of its binary
   weight.  The round stops when a zero count is appended.  The
   function returns a(n) for any non‑negative n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_counts_cap.h"

/* Count the number of bits set in a seq_t value. */
static unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Ensure seq has capacity for at least need elements. */
static void ensure_seq_cap(size_t *cap, seq_t **array, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        *array = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static storage for the generated sequence and counts per weight. */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static size_t *wcnt  = NULL;
    static size_t wcnt_len = 0;

    size_t nsize = (size_t)n;          /* index in 0‑based terms */

    /* Initialisation on first call. */
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)0;            /* a(0) = 0 */
        seq_len = 1;
        /* Counts of binary weights in the current sequence. */
        wcnt_len = 1;
        wcnt = (size_t *)malloc(wcnt_len * sizeof(size_t));
        wcnt[0] = 1;                  /* weight of a(0) is 0 */
    }

    /* Generate terms until we have at least nsize+1 terms. */
    while (seq_len <= nsize) {
        size_t w = 0;
        for (;;) {
            /* Ensure the counts array covers this weight. */
            ensure_counts_cap(&wcnt_len, &wcnt, w);

            size_t count = wcnt[w];    /* number of terms with weight w */

            /* Append the count as the next term. */
            ensure_seq_cap(&seq_cap, &seq, seq_len + 1);
            seq[seq_len++] = (seq_t)count;

            /* Update counts for the weight of the new term. */
            unsigned pw = popcount_seq((seq_t)count);
            ensure_counts_cap(&wcnt_len, &wcnt, pw);
            ++wcnt[pw];

            /* Terminate this round when we appended a zero term. */
            if (count == 0) break;

            ++w;
        }
    }

    return seq[nsize];
}
