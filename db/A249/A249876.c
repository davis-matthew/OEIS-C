/*
 * A249876  Pseudo-lucky numbers.
 * https://oeis.org/A249876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pseudo‑lucky numbers.  This code builds the sequence by the standard
   sieve described in the OEIS entry: start with the natural numbers,
   then repeatedly delete every k‑th remaining number where k is the
   k‑th surviving number (beginning at k = 2).  a(n) is the n‑th
   surviving number. */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
  #define SEQ_ALLOC(n)   new seq_t[(n)]
  #define SEQ_FREE(p)    delete[] (p)
#else
  #define SEQ_ALLOC(n)   ((seq_t*)malloc((n) * sizeof(seq_t)))
  #define SEQ_FREE(p)    free(p)
#endif

/* --------------------------------------------------------------- */
/*   Static cache holding all surviving numbers computed so far. */
static seq_t *pseudo_lucky_cache = NULL;   /* array of surviving numbers   */
static size_t pseudo_lucky_count = 0;       /* number of survivors stored  */

/* --------------------------------------------------------------- */
/*  Recompute the sequence up to at least 'min_needed' survivors. */
static void make_cache(idx_t min_needed)
{
    if (min_needed <= pseudo_lucky_count)
        return;                                 /* already enough */

    if (pseudo_lucky_cache) {
        SEQ_FREE(pseudo_lucky_cache);
        pseudo_lucky_cache = NULL;
        pseudo_lucky_count = 0;
    }

    /* Rough upper bound that should produce more than min_needed
       survivors for 10⁴ terms.  The factor 30 gives about 3×10⁵
       numbers, comfortably covering the required range. */
    size_t upper = 30 * (size_t)min_needed + 100;

    seq_t *arr = SEQ_ALLOC(upper);
    for (size_t i = 1; i <= upper; ++i)
        arr[i - 1] = (seq_t)i;                 /* numbers 1 … upper */

    size_t len  = upper;                       /* current survivor count */
    size_t pos  = 1;                           /* index of next step (0‑based) */

    while (pos < len) {
        seq_t step_seq = arr[pos];
        size_t step = (size_t)step_seq;
        if (step > len)                        /* no further deletions */
            break;

        seq_t *next = SEQ_ALLOC(len);          /* temporary array       */
        size_t new_len = 0;

        for (size_t i = 0; i < len; ++i)
            if ( ((i + 1) % step) != 0 )
                next[new_len++] = arr[i];

        SEQ_FREE(arr);
        arr  = next;
        len  = new_len;
        ++pos;                                 /* advance to next step   */
    }

    pseudo_lucky_cache = arr;
    pseudo_lucky_count = len;                  /* store number of survivors */
}

/* --------------------------------------------------------------- */
/*  Required entry point:  a(idx_t n) returns the n‑th pseudo‑lucky
    number.  The sequence is offset 1, so a(1) = 1. */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    make_cache(n);                                 /* ensure enough terms */
    return pseudo_lucky_cache[n - 1];
}
