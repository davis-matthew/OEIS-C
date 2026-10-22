/*
 * A336025  Numbers m providing record values for the proportion of
 *          nonsquarefree integers in the interval [1, m].
 * https://oeis.org/A336025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Record values for the proportion of nonsquarefree integers in the interval [1, m] */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static state */
    static seq_t *seq = NULL;      /* array of recorded values */
    static size_t seq_len = 0;     /* number of recorded values */
    static size_t seq_cap = 0;     /* allocated capacity */
    static idx_t max_checked = 0;  /* up to which m we have inspected */
    static idx_t count_nonsq = 0;  /* number of nonsquarefree numbers up to max_checked */
    static idx_t max_count = 0;    /* count for the current record */
    static idx_t max_m_record = 1; /* m at which current record occurs */

    /* Generate more records until we have at least n values or reach a safe limit */
    while (seq_len < n) {
        if (max_checked >= 100000) {
            /* No further records are expected beyond 100 000 for this sequence */
            break;
        }
        ++max_checked;

        /* Determine if max_checked is divisible by any square > 1 */
        bool nonsq = false;
        for (idx_t i = 2; i * i <= max_checked; ++i) {
            if (max_checked % (i * i) == 0) {
                nonsq = true;
                break;
            }
        }
        if (nonsq) {
            ++count_nonsq;
        }

        /* Check if we now have a new record for the ratio of nonsquarefree numbers */
        /* Compare count_nonsq / max_checked with max_count / max_m_record */
        if (count_nonsq * max_m_record > max_count * max_checked) {
            /* Ensure capacity of the record array */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            /* Store the new record */
            seq[seq_len++] = (seq_t)max_checked;
            max_count = count_nonsq;
            max_m_record = max_checked;
        }
    }

    /* Return the requested term if it exists, otherwise zero */
    if (n <= seq_len) {
        return seq[n - 1];
    } else {
        return (seq_t)0;
    }
}
