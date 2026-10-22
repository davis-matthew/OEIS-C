/*
 * A392476  a(n) = a(n-1) - a(a(n-1)) if a(n-1) has appeared exactly once;
 *          otherwise a(n) = number of times a(n-1) has appeared; a(0) = 0.
 * https://oeis.org/A392476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0
   For n > 0:
     let prev  = a(n-1)
     let occ   = number of occurrences of prev in a[0 .. n-1]
     if occ == 1:   a(n) = prev - a(prev)
     else:          a(n) = occ
*/

#include <stddef.h>
#include <stdlib.h>

/* static storage for the generated terms */
static seq_t *seq      = NULL;   /* array of terms */
static size_t seq_cap  = 0;      /* current allocated capacity */
static idx_t  max_n_computed = 0; /* highest n for which a(n) has been calculated */

/* ensure that seq is large enough to hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (seq == NULL) {
        /* first call: initialise storage */
        seq_cap = 4;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)0;          /* a(0) = 0 */
        max_n_computed = 0;
    }

    if (n <= max_n_computed)
        return seq[n];              /* already computed */

    /* grow array to hold up to n */
    ensure_capacity(n);

    /* compute terms from max_n_computed+1 up to n */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];

        /* count occurrences of prev in a[0 .. i-1] */
        idx_t cnt = 0;
        for (idx_t j = 0; j < i; ++j)
            if (seq[j] == prev)
                ++cnt;

        if (cnt == 1) {
            /* need a(prev); ensure prev is an index */
            idx_t prev_idx = (idx_t)prev;
            if (prev_idx > max_n_computed) {
                /* compute a(prev_idx) recursively (will update max_n_computed) */
                a(prev_idx);
            }
            seq_t inner = seq[prev_idx];
            seq[i] = prev - inner;
        } else {
            seq[i] = (seq_t)cnt;
        }
    }

    max_n_computed = n;
    return seq[n];
}
