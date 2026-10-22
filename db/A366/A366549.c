/*
 * A366549  a(0) = 0; for n > 0, a(n) is the number of terms prior to and
 *          including the term a(n-1-a(n-1)) that equal a(n-1-a(n-1)).
 * https://oeis.org/A366549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 6219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366549
   a(0) = 0
   for n > 0, let k = n-1-a(n-1)
   then a(n) = #{i | 0 <= i <= k and a(i) = a(k)}      (inclusive)
   The value a(k) is a count and therefore always < = k,
   so it safely fits in idx_t.                       */

#include <stdlib.h>

static seq_t *seq = NULL;     /* array of sequence values          */
static idx_t  seq_cap = 0;    /* allocated capacity (size)        */
static idx_t  seq_len = 0;    /* number of values that have been   */
/*   computed and stored in seq[0 .. seq_len-1]               */

/* Ensure the array has room for indices 0 … n           */
static void ensure_seq_cap(idx_t n)
{
    if (n < seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Return the n-th term of the sequence                     */
seq_t a(idx_t n)
{
    /* initialise the first term lazily                        */
    if (n == 0) {
        if (seq_len == 0) {
            ensure_seq_cap(0);
            seq[0] = (seq_t)0;
            seq_len = 1;
        }
        return seq[0];
    }

    /* if the value is already computed, just return it      */
    if (n < seq_len) return seq[n];

    /* extend storage to accommodate index n                */
    ensure_seq_cap(n);

    /* compute all missing terms up to n                     */
    for (idx_t i = seq_len; i <= n; ++i) {
        /* value a(i-1) is guaranteed to be <= i-1           */
        idx_t prev = (idx_t)seq[i - 1];
        idx_t k    = (i - 1) - prev;         /* k ∈ [0, i-1]          */
        seq_t v    = seq[k];

        /* count occurrences of v up to and including k     */
        idx_t count = 0;
        for (idx_t j = 0; j <= k; ++j)
            if (seq[j] == v) ++count;

        seq[i] = (seq_t)count;
    }
    seq_len = n + 1;
    return seq[n];
}
