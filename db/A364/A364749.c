/*
 * A364749  a(1) = 1. Thereafter, if a(n-1) is a novel term a(n) = a(a(n-1)),
 *          otherwise a(n) is the number of times a(n-1) has been repeated.
 * https://oeis.org/A364749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, if a(n-1) is novel (first occurrence) then
 *            a(n) = a(a(n-1)), otherwise (repeated) a(n) = k-1 where k is the
 *            total occurrences of a(n-1) up to this point
 * Range:     u8 n <= 4362, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Static storage for the sequence and value occurrence counts.         */
static seq_t   *seq       = NULL;      /* sequence, indexed from 1       */
static size_t  seq_cap    = 0;          /* capacity (indices 0..seq_cap)   */
static idx_t   computed_upto = 0;       /* largest index computed so far   */

static size_t *val_counts = NULL;      /* occurrence count per value      */
static size_t  val_cap    = 0;          /* capacity for val_counts         */

/* --------------------------------------------------------------------- */
/* Ensure capacity for the sequence array.                                */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Ensure capacity for the value counts array.                           */
static void ensure_counts_cap(idx_t need)
{
    if (val_cap >= (size_t)need) return;
    size_t new_cap = val_cap ? val_cap << 1 : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;
    size_t *new_ptr = (size_t *)realloc(val_counts, (new_cap + 1) * sizeof(size_t));
    /* zero the newly allocated segment */
    if (new_cap > val_cap)
        memset(new_ptr + val_cap + 1, 0, (new_cap - val_cap) * sizeof(size_t));
    val_counts = new_ptr;
    val_cap    = new_cap;
}

/* --------------------------------------------------------------------- */
/* Compute the sequence up to index limit (inclusive).                    */
static void compute_up_to(idx_t limit)
{
    while (computed_upto < limit) {
        idx_t i = computed_upto + 1;          /* next index to compute   */
        if (i == 1) {
            /* initial term a(1) = 1 */
            ensure_seq_cap(1);
            seq[1] = (seq_t)1;
            ensure_counts_cap(1);
            val_counts[1] += 1;                 /* first occurrence      */
            computed_upto = 1;
            continue;
        }

        seq_t prev = seq[i - 1];
        idx_t prev_idx = (idx_t)prev;
        ensure_counts_cap(prev_idx);
        size_t cnt = val_counts[prev_idx];

        seq_t next;
        if (cnt == 1) {                       /* novel previous term    */
            /* ensure the referenced index has been computed */
            if (prev_idx > computed_upto)
                compute_up_to(prev_idx);
            next = seq[prev_idx];
        } else {                               /* repeated previous term */
            next = (seq_t)(cnt - 1);
        }

        /* store the new term */
        ensure_seq_cap(i);
        seq[i] = next;

        /* update count for the new term */
        idx_t next_idx = (idx_t)next;
        ensure_counts_cap(next_idx);
        val_counts[next_idx] += 1;

        computed_upto = i;
    }
}

/* --------------------------------------------------------------------- */
/* External function required by the exercise.                           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* out of range, safety    */
    compute_up_to(n);
    return seq[n];
}
