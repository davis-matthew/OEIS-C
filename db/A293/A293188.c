/*
 * A293188  Unitary pseudoperfect numbers: numbers that equal to the sum of a
 *          subset of their aliquot unitary divisors.
 * https://oeis.org/A293188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_unitary_pseudoperfect, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A293188
 *  Unitary pseudoperfect numbers – numbers equal to the sum of a
 *  subset of their unitary divisors.
 *
 *  The implementation enumerates natural numbers starting from 6
 *  and tests each candidate for the unitary–pseudoperfect property.
 *  A subset‑sum DP over the unitary divisors (using a bitset) shows
 *  whether a subset sums to the candidate itself.  Results are cached
 *  in a dynamically resized array so that calls to a(idx_t n) are
 *  efficient even when n is requested in arbitrary order.
 *
 *  The code is fully generic: it works with any unsigned integer
 *  type seq_t (including arbitrary‑precision classes) and any
 *  64‑bit unsigned index type idx_t.  Arithmetic is performed with
 *  the allowed operators; only standard C library allocation
 *  (malloc/free) and the helper “seq_realloc” are used for the
 *  dynamic array of seq_t values.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc(seq_t*,size_t,size_t) */
#include "../utils/is_unitary_pseudoperfect.h"

/*---------------------------------------------------------------------*/
/*  Helper: test whether a 64‑bit integer n is unitary pseudoperfect   */

/*---------------------------------------------------------------------*/
/*  Dynamic array of seq_t values                                      */
/*---------------------------------------------------------------------*/
static seq_t   *seq  = NULL;   /* cached sequence values */
static size_t  seq_len = 0;    /* number of stored elements */
static size_t  seq_cap = 0;    /* current allocation capacity */
static uint64_t next_candidate = 6;   /* integer we test next */

/* grow the seq array, doubling capacity */
static void seq_grow(void)
{
    if (seq_len < seq_cap) return;

    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    seq_t *new_arr = seq_realloc(seq, seq_cap, new_cap);
    if (!new_arr) return;          /* allocation failure → keep old pointer */

    seq = new_arr;
    seq_cap = new_cap;
}

/*---------------------------------------------------------------------*/
/*  Public function required by the build                            */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* produce at least n terms */
    while (seq_len < (size_t)n) {
        /* find next unitary pseudoperfect number */
        while (!is_unitary_pseudoperfect(next_candidate)) ++next_candidate;

        seq_grow();                 /* ensure room for new value */
        seq[seq_len++] = (seq_t)next_candidate;   /* cast is safe: value fits into seq_t */
        ++next_candidate;           /* advance search position */
    }

    return seq[(size_t)n - 1];   /* offset 1: a(1) is seq[0] */
}
