/*
 * A327616  Irregular table read by rows; the first row contains a single 1;
 *          for any n > 1, row n+1 corresponds to the ordinal transform of the
 *          terms in rows 1..n.
 * https://oeis.org/A327616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6339, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A327616 : irregular table read by rows
 *
 *  The sequence is built by storing all rows of the irregular table
 *  in a flat array.  Row 1 is [1].  For each subsequent row we
 *  apply the “ordinal transform” to the entire previous sequence:
 *  for each element x, its new value is the number of times x
 *  has already appeared in the sequence.  The new row is appended
 *  to the existing sequence.  The table has the property that
 *  the length doubles each time.
 *
 *  Algorithm
 *  ---------
 *  * Keep a static `seq_t *seq` that stores the full sequence.
 *  * `seq_len` – current number of stored terms.
 *  * `seq_cap` – allocated capacity of `seq`.
 *  * `max_val` – maximum value seen so far (used to size the
 *    frequency table).
 *  * `freq` – array `size_t *` that counts occurrences of the
 *    values while building a new row.  Its size is always
 *    at least `max_val+1`.
 *
 *  When `a(n)` is called, the routine expands the stored
 *  sequence until it contains at least `n` terms, then
 *  returns `seq[n-1]`.  The routine works for arbitrary
 *  call order; it uses memoisation to avoid recomputation.
 *
 *  The code is valid C11 and C++17.  It relies only on
 *  standard integer operators and the utility
 *  function `alloc_seq()` that provides a properly typed
 *  pointer for `seq_t`.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;       /* packed sequence */
    static idx_t seq_len = 0, seq_cap = 0;
    static seq_t max_val = 0;

    static size_t *freq = NULL;     /* occurrence counter */
    static idx_t freq_cap = 0;

    /* ---------- initialise on first call ---------- */
    if (!seq) {
        seq_cap = 2;
        seq_len = 1;
        seq = alloc_seq(seq_cap);
        seq[0] = (seq_t)1;
        max_val = (seq_t)1;

        freq_cap = 2;
        freq = (size_t *)malloc(freq_cap * sizeof(size_t));
    }

    /* ---------- extend the sequence until we have n terms ---------- */
    while (seq_len < n) {
        idx_t old_len = seq_len;
        idx_t new_len = old_len + old_len;            /* double the size */

        /* enlarge storage for the new terms if necessary */
        if (new_len > seq_cap) {
            seq_cap = new_len;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        /* enlarge frequency table to accommodate any value already seen */
        idx_t needed = (idx_t)max_val + 1;
        if (needed > freq_cap) {
            freq_cap = needed;
            freq = (size_t *)realloc(freq, freq_cap * sizeof(size_t));
        }

        /* initialise frequency counts for this row */
        memset(freq, 0, needed * sizeof(size_t));

        /* build the new row – ordinal transform of the prefix */
        for (idx_t i = 0; i < old_len; ++i) {
            seq_t val   = seq[i];
            size_t cnt  = freq[(size_t)val];
            size_t next = cnt + 1;

            seq[old_len + i] = (seq_t)next;
            freq[(size_t)val] = next;

            if (seq[old_len + i] > max_val)
                max_val = seq[old_len + i];
        }

        seq_len = new_len;
    }

    return seq[n - 1];
}
