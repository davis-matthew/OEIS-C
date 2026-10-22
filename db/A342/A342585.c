/*
 * A342585  Inventory sequence: record the number of zeros thus far in the
 *          sequence, then the number of ones thus far, then the number of
 *          twos thus far and so on, until a zero is recorded; the inventory
 *          then starts again, recording the number of zeros.
 * https://oeis.org/A342585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 5438, u16 n <= 9591, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: inventory sequence A342585
 * Generation rule:
 *   Start a new inventory cycle.
 *   For i=0,1,2,... output the number of occurrences of i so far
 *   in the sequence.  When a 0 is output the current inventory ends,
 *   and a new one starts after that point.
 *
 * This implementation memoises the sequence in a dynamically grown
 * array.  The frequency table is also grown as needed.  The code is
 * valid C11 and C++17; arbitrary‑precision seq_t types are handled via
 * the provided allocation utilities.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc */

/* allocation utilities for seq_t (may be an arbitrary‑precision class) */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* The function as required by the build system. */
seq_t a(idx_t n)
{
    /* ---------- memoised sequence ---------------------------------- */
    static seq_t *seq = NULL;        /* array of computed terms   */
    static size_t seq_cap = 0;       /* current capacity of seq   */
    static idx_t seq_len = 0;        /* number of terms stored    */

    /* ---------- frequency table ----------------------------------- */
    static idx_t *freq = NULL;       /* freq[x] = # of occurrences of value x */
    static size_t freq_cap = 0;      /* current capacity of freq  */

    /* ---------- inventory state ----------------------------------- */
    static bool seg_in_progress = false; /* true if inside an inventory block */
    static idx_t seg_i = 0;                  /* current i of the inventory */

    /* ---------- initialise static structures (first call) ---------- */
    if (!seq) {
        seq_cap = 1;
        seq      = seq_alloc_new_array(seq_cap);   /* constructs seq elements */
    }
    if (!freq) {
        freq_cap = 64;
        freq = (idx_t*)malloc(sizeof(idx_t) * freq_cap); /* primitive type */
        for (size_t i = 0; i < freq_cap; ++i)
            freq[i] = 0;
    }

    /* ---------- generate terms up to position n ------------------- */
    while (seq_len < n) {
        /* start a new inventory block if the previous one finished */
        if (!seg_in_progress) {
            seg_in_progress = true;
            seg_i = 0;
        }

        /* current count to output: number of occurrences of seg_i */
        idx_t cnt = (seg_i < freq_cap) ? freq[seg_i] : 0;

        /* ensure seq has space for one more element */
        if ((size_t)seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *new_seq = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < seq_len; ++i)
                new_seq[i] = seq[i];
            seq_free_array(seq);
            seq = new_seq;
            seq_cap = new_cap;
        }

        /* store the count */
        seq[(size_t)seq_len++] = (seq_t)cnt;

        /* ensure freq table can hold cnt as an index */
        idx_t cnt_idx = cnt;
        if (cnt_idx >= freq_cap) {
            size_t new_cap = freq_cap;
            while (new_cap <= (size_t)cnt_idx) new_cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(freq, sizeof(idx_t) * new_cap);
            if (!tmp) { /* allocation failure – abort gracefully */
                seq_len = 0;
                seq_cap = 0;
                seq = NULL;
                freq_cap = 0;
                free(freq);
                freq = NULL;
                return (seq_t)0;
            }
            /* zero initialise newly added slots */
            for (size_t i = freq_cap; i < new_cap; ++i)
                tmp[i] = 0;
            freq = tmp;
            freq_cap = new_cap;
        }

        /* update frequency of the newly appended value */
        ++freq[cnt_idx];

        /* if we just appended a zero, the current inventory block ends */
        if (cnt == 0)
            seg_in_progress = false;
        else
            ++seg_i;   /* next i in this inventory block */
    }

    /* The sequence is 1‑indexed; return the element at position n-1. */
    return seq[(size_t)(n - 1)];
}
