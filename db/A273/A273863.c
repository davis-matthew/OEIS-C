/*
 * A273863  The successive numbers of integers visible between two even terms
 *          are given by the sequence itself.
 * https://oeis.org/A273863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construct A273863 by a greedy algorithm that preserves the gaps
   between consecutive even terms as given by the permutation itself.
   The sequence is built incrementally, keeping the next unused odd
   and even numbers, and a counter for how many numbers must appear
   before the next even. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq = NULL;
static size_t seq_cap = 0;            /* capacity of seq array          */
static idx_t computed_max = 0;        /* number of terms already computed */

static idx_t even_cnt    = 0;          /* how many evens have been placed */
static idx_t remaining_gap = 0;       /* numbers left to insert before next even */
static idx_t next_odd    = 1;         /* next unused odd number         */
static idx_t next_even   = 2;         /* next unused even number       */

/* Ensure the seq array can hold at least 'need' elements (indices are 1‑based). */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Compute and return the nth term of A273863 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1, return 0 */

    if (computed_max < n) {
        if (seq == NULL) {                 /* first call: initialise */
            ensure_seq_cap(n + 1);
            seq[1] = (seq_t)0;            /* a(1) = 0 */
            even_cnt = 1;                 /* first even placed */
            remaining_gap = (idx_t)seq[1];/* a(1) = 0 -> no numbers before next even */
            next_odd  = 1;                /* start odd sequence */
            next_even = 2;                /* next unused even */
            computed_max = 1;
        }

        while (computed_max < n) {
            ++computed_max;

            if (remaining_gap > 0) {
                /* insert the next odd number */
                seq[computed_max] = (seq_t)next_odd;
                next_odd += 2;            /* keep odd parity */
                --remaining_gap;
            } else {
                /* insert the next even number */
                seq[computed_max] = (seq_t)next_even;
                next_even += 2;           /* keep even parity */
                ++even_cnt;               /* this even is the even_cnt‑th even term */
                remaining_gap = (idx_t)seq[even_cnt]; /* new gap length */
            }
        }
    }

    return seq[n];
}
