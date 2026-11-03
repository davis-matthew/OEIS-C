/*
 * A345919  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum < 0.
 * https://oeis.org/A345919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A345919:
 *   a(n) = the n‑th non‑negative integer k such that the
 *   k‑th composition in standard order (A066099) has
 *   alternating sum < 0.
 * The algorithm iterates k from 0 upwards, tests each,
 * and stores the values that satisfy the condition in a
 * static cache.  This yields O(n) time for the n‑th term
 * and is independent of the order of calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Compute the alternating sum of the k‑th composition
 * defined by standard order A066099.
 *
 * Composition of k:
 *   Let rev be the binary expansion of k written with the
 *   most significant bit first (reverse of usual order).
 *   The positions of 1’s in rev, counted from 1 at the
 *   leftmost bit, together with a leading 0, give a sorted
 *   list of indices.  First differences of this list form
 *   the un‑reversed parts of the composition, which we
 *   reverse again to obtain the true part sizes.
 *
 * The function returns a signed 64‑bit integer that is the
 * alternating sum of that composition (positive or
 * negative). */
static inline int64_t alternating_sum(idx_t k)
{
    int64_t sum = 0;          /* running sum of signed parts   */
    int    sign = -1;         /* sign for the next part: -1, +1, ... */
    idx_t  pos    = 1;         /* bit position in the *original* k,
                                 counted from the LSB   */
    idx_t  prev   = 0;         /* position of previous 1‑bit   */
    idx_t  t      = 0;         /* number of 1‑bits found       */

    while (k)
    {
        if (k & 1ULL)
        {
            /* part size = difference between current
               1‑bit position and previous such position */
            int64_t part = (int64_t)pos - (int64_t)prev;
            sum  += sign * part;
            sign = -sign;
            prev  = pos;
            ++t;
        }
        k >>= 1;
        ++pos;
    }

    /* If number of parts is odd, the computed sum needs
       a final sign flip to become the alternating sum
       of the *reversed* composition. */
    if (t & 1ULL)
        return -sum;
    return sum;
}

/* Main entry point: sequence a(n) */
seq_t a(idx_t n)
{
    /* Static cache of k's satisfying the condition. */
    static seq_t *good = NULL;
    static size_t len  = 0;    /* number of stored k's     */
    static size_t cap  = 0;    /* allocated capacity       */

    /* The next integer to test.  It grows only when needed. */
    static idx_t next_k = 0;

    if (n == 0)
        return 0;   /* undefined; guard against misuse */

    /* Grow the cache until it contains at least n values. */
    while (len < n)
    {
        idx_t k = next_k++;
        if (alternating_sum(k) < 0)
        {
            /* Store the value k – cast to seq_t for safety. */
            append_candidate(&good, &len, &cap, (seq_t)k);
        }
    }

    return good[n - 1];
}
