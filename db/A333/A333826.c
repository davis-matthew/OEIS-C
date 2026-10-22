/*
 * A333826  a(1)=1; for n>1, a(n) = the greatest common divisor (GCD) of n and
 *          the sum of all previous terms if the GCD is not already in the
 *          sequence; otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A333826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 242, u32 n <= 5399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333826:
   a(1) = 1;
   For n > 1:
     s   = a(1)+a(2)+...+a(n-1);
     g   = gcd(n, s);
     if g has appeared among a(1)..a(n-1)  then
           a(n) = a(n-1) + n
     else a(n) = g
   Indexing starts at 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;   /* 1‑indexed array of terms, seq[0] unused   */
static size_t sz   = 0;     /* number of terms currently stored         */
static size_t cap  = 0;     /* number of elements allocated (0‑based)    */
static seq_t cum_sum = 0;   /* running sum of all stored terms          */

/* Ensure that seq has room for elements up to index `max_index` (1‑based). */
static void ensure_capacity(idx_t max_index) {
    /* max_index is the largest array index that will be accessed. */
    if (max_index < (idx_t)cap) return;                /* already enough */
    size_t newcap = cap ? cap << 1 : 16;               /* start with 16 if currently empty  */
    /* newcap must be strictly greater than max_index */
    while (newcap <= (size_t)max_index) newcap <<= 1;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Return a(n), building the sequence on demand. */
seq_t a(idx_t n) {
    if (n < 1) return 0;      /* not defined for n < 1, but guard anyway */

    /* Build terms up to index n if necessary. */
    while (sz < n) {
        idx_t next = sz + 1;                    /* next index to compute (1‑based) */
        seq_t g = gcd((seq_t)next, cum_sum);    /* candidate gcd               */

        /* Check if g already appeared among previous terms. */
        bool seen = false;
        for (idx_t i = 1; i <= (idx_t)sz; ++i) {
            if (seq[i] == g) { seen = true; break; }
        }

        seq_t val = seen ? seq[sz] + (seq_t)next : g;  /* compute a(next) */

        /* Store the new term. */
        ensure_capacity(next);
        seq[next] = val;
        cum_sum += val;
        ++sz;
    }

    return seq[n];
}
