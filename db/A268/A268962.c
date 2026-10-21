/*
 * A268962  a(1) = 1 and a(2) = 2; thereafter a(n+1) = floor[sqrt(a(n))] if
 *          not already in the sequence; otherwise a(n+1) = a(n) + a(n-1).
 * https://oeis.org/A268962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; For n > 2: let v = floor(sqrt(a(n-1))); if v
 *            has NOT appeared earlier in the sequence, a(n)=v; otherwise
 *            a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 5361, u64 n <= 9631,
 *            u128 n <= 9631, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq       = NULL;   /* 1‑based sequence storage */
static size_t seq_len   = 0;      /* highest computed index */
static size_t seq_cap   = 0;      /* allocated capacity */

/* Grow the storage until it can hold at least 'needed' indices (1‑based). */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n‑th term of A268962 (n ≥ 1).  The function is memoised
 * and works for calls in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard */

    /* Initialise the first two terms on the first call. */
    if (seq_len == 0) {
        ensure_capacity(4);          /* need indices 1 and 2 at least */
        seq[1] = 1;
        seq[2] = 2;
        seq_len = 2;
    }

    /* If the requested term is already known, return it. */
    if (n <= seq_len) return seq[n];

    /* Make sure there is room for the new terms. */
    ensure_capacity((size_t)n + 1);  /* indices are 1‑based */

    /* Compute all missing terms up to n. */
    for (idx_t k = seq_len + 1; k <= n; ++k) {
        seq_t cand = floor_sqrt(seq[k - 1]);   /* floor(sqrt(a(k‑1))) */

        /* Check whether cand has already appeared in the sequence. */
        int present = 0;
        for (idx_t i = 1; i < k; ++i) {
            if (seq[i] == cand) { present = 1; break; }
        }

        if (!present)
            seq[k] = cand;
        else
            seq[k] = seq[k - 1] + seq[k - 2];
    }

    seq_len = n;
    return seq[n];
}
