/*
 * A268696  a(1)=1; thereafter a(n+1) = floor(m/2), where m = number of
 *          occurrences of a(n) in [a(1),...,a(n)].
 * https://oeis.org/A268696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(1) = 1 a(n+1) = floor( m / 2 ) where m = number of occurrences
 *            of a(n) in [a(1),…,a(n)]. (OEIS A268696) The implementation
 *            memoises the sequence and a count table for occurrences of each
 *            observed value. The count table is indexed by the numeric value
 *            of a(n) (converted to idx_t); values are gua...
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h> /* for uint64_t (idx_t likely defined elsewhere) */

seq_t a(idx_t n)
{
    /* 1‑based allocated sequence storage */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;   /* number of terms already computed */
    static idx_t seq_cap = 0;   /* current allocation size (1‑based) */

    /* Count array: counts[value] = number of occurrences of that value
       so far.  Indexed by the numeric value converted to idx_t. */
    static idx_t *cnt = NULL;
    static idx_t cnt_cap = 0;   /* allocation size of cnt (0‑based) */

    /* ----- Lazy initialisation ----- */
    if (seq_len == 0) {
        /* start with a modest capacity */
        seq_cap = 8;
        seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;            /* allocation failure */

        seq_len = 1;
        seq[1] = (seq_t)1;                    /* a(1) = 1 */

        /* count array needs indices 0 and 1 */
        cnt_cap = 2;
        cnt     = (idx_t*)malloc(cnt_cap * sizeof(idx_t));
        if (!cnt) { free(seq); return (seq_t)0; }
        cnt[0] = 0;
        cnt[1] = 1;      /* value 1 has occurred once */

        return seq[1];
    }

    /* If the requested index is already computed, just return it */
    if (n <= seq_len)
        return seq[n];

    /* ---- Compute terms up to n ---- */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* previous term a(i-1) */
        seq_t prev_val = seq[i - 1];
        idx_t idx_prev = (idx_t)prev_val;      /* safe, values are small */

        /* Ensure count array covers this value */
        while (idx_prev >= cnt_cap) {
            idx_t new_cap = cnt_cap * 2;
            if (new_cap <= cnt_cap) new_cap = idx_prev + 1; /* overflow guard */
            idx_t *tmp = (idx_t*)realloc(cnt, new_cap * sizeof(idx_t));
            if (!tmp) { free(seq); free(cnt); return (seq_t)0; }
            /* initialise new entries to 0 */
            for (idx_t j = cnt_cap; j < new_cap; ++j) tmp[j] = 0;
            cnt = tmp;
            cnt_cap = new_cap;
        }

        idx_t occ = cnt[idx_prev];          /* m = occurrences of a(i-1) */
        idx_t next_idx = occ / (idx_t)2;    /* floor division */
        seq_t next_val = (seq_t)next_idx;

        /* Ensure sequence array capacity */
        if (seq_len + 1 >= seq_cap) {
            idx_t new_cap = seq_cap * 2;
            if (new_cap <= seq_cap) new_cap = seq_len + 2; /* overflow guard */
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) { free(seq); free(cnt); return (seq_t)0; }
            seq = tmp;
            seq_cap = new_cap;
        }

        seq_len++;
        seq[seq_len] = next_val;            /* store a(i) */

        /* Update count for the new value */
        idx_t idx_next = (idx_t)next_val;
        while (idx_next >= cnt_cap) {
            idx_t new_cap = cnt_cap * 2;
            if (new_cap <= cnt_cap) new_cap = idx_next + 1;
            idx_t *tmp = (idx_t*)realloc(cnt, new_cap * sizeof(idx_t));
            if (!tmp) { free(seq); free(cnt); return (seq_t)0; }
            for (idx_t j = cnt_cap; j < new_cap; ++j) tmp[j] = 0;
            cnt = tmp;
            cnt_cap = new_cap;
        }
        cnt[idx_next] += (idx_t)1;
    }

    return seq[n];
}
