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
 * Helpers:   alternating_sum, append_candidate
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
#include "../utils/append_candidate.h"
#include "../utils/alternating_sum.h"

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
