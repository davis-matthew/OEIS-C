/*
 * A386963  Gaps between positions of odd terms in A065090.
 * https://oeis.org/A386963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386963: Gaps between positions of odd terms in A065090
   Formula: a(n) = odd_pos[n] - odd_pos[n-1], where odd_pos[k] is the
   position in A065090 of the (k‑th) odd non‑prime term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   // provides is_prime(idx_t)

/* -------------------------------------------------------------------------- */
/* Storage for the positions of odd non‑prime terms (1‑based positions in A065090). */
static size_t odd_cap     = 0;          /* allocated capacity */
static size_t odd_count   = 0;          /* number of stored odd positions */
static idx_t *odd_pos = NULL;           /* array of positions */

/* Ensure odd_pos has at least needed slots. */
static void grow_odd_pos(size_t needed)
{
    if (odd_cap >= needed) return;
    size_t new_cap = odd_cap ? odd_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(odd_pos, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    odd_pos = tmp;
    odd_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Current generator state: next integer to inspect (m) and current position in A065090 (pos). */
static idx_t next_m = 1;   /* next integer to evaluate */
static idx_t cur_pos = 1;  /* position in the full sequence A065090 */

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; return 0 for n==0 safely */
        return (seq_t)0;

    /* Generate terms until we have at least n+1 odd positions (index 0…n). */
    while (odd_count <= n) {
        /* Determine whether the current integer next_m belongs to the sequence. */
        if (!( (next_m & 1u) && is_prime(next_m) )) {  /* not an odd prime */
            /* It's part of A065090. Check if it's an odd non‑prime (i.e., an odd composite or 1). */
            if ((next_m & 1u)) {
                grow_odd_pos(odd_count + 1);
                odd_pos[odd_count++] = cur_pos;   /* store its position */
            }
            cur_pos++;          /* advance to next position in the sequence */
        }
        next_m++;              /* move to the next integer for evaluation */
    }

    seq_t diff = odd_pos[n] - odd_pos[n - 1];
    return diff;
}
