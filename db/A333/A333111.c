/*
 * A333111  a(n) is the least positive integer not used earlier such that n+1
 *          and the sum of terms up to a(n) are coprime.
 * https://oeis.org/A333111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 717, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the least unused positive integer x such that
   gcd( sum_{i=1}^n a(i), n+1 ) = 1.  a(1)=1. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Static cache for computed sequence values and bookkeeping. */
static seq_t *seq      = NULL;   /* array of terms */
static size_t  seq_cap = 0;      /* capacity of seq   */
static idx_t   seq_len = 0;      /* number of terms stored */

static bool *used      = NULL;   /* used[ x ] == true if x already appeared */
static size_t used_cap  = 0;      /* capacity of used */

static seq_t total_sum = 0;      /* running sum of terms */

/* Ensure an array has capacity for at least `needed` elements.
   The newly allocated part is zero‑initialised. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    void *p = realloc(*ptr, new_cap * elem_size);
    if (!p) { /* allocation failed; abort */ abort(); }
    /* Initialise the new part to zero. */
    if (new_cap > *cap) {
        size_t offset = *cap * elem_size;
        size_t len    = (new_cap - *cap) * elem_size;
        memset((char *)p + offset, 0, len);
    }
    *ptr = p;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safe guard for non‑positive indices */

    /* Extend the cached sequence up to the requested n. */
    for (idx_t pos = seq_len + 1; pos <= n; ++pos) {
        idx_t candidate = 1;

        /* Find the smallest unused integer satisfying the coprimality condition. */
        while (1) {
            if (candidate >= used_cap) {
                /* Grow the used flag array to include this candidate. */
                ensure_capacity((size_t)candidate + 1, &used_cap,
                                (void **)&used, sizeof(bool));
            }

            if (!used[(size_t)candidate] &&
                gcd(total_sum + (seq_t)candidate, (seq_t)(pos + 1)) == 1) {
                break;  /* found the term a(pos) */
            }
            ++candidate;
        }

        /* Store the new term. */
        if (seq_len >= seq_cap) {
            ensure_capacity(seq_len + 1, &seq_cap,
                            (void **)&seq, sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)candidate;

        /* Update bookkeeping. */
        total_sum += (seq_t)candidate;
        used[(size_t)candidate] = true;
    }

    /* seq_len >= n now. Return the requested term. */
    return seq[n - 1];
}
