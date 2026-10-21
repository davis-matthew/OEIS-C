/*
 * A296871  Numbers whose base-6 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_more_pits_than_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296871
   Formula: a(n) is the nth integer whose base‑6 digits have
   more pits (d(i-1) > d(i) < d(i+1)) than peaks (d(i-1) < d(i) > d(i+1)).
   The code generates the sequence by brute‑force scanning of integers,
   storing the found terms in a static dynamic array. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_more_pits_than_peaks.h"

seq_t a(idx_t n)
{
    /* Static state for memoization. */
    static seq_t   *data = NULL;   /* dynamic array of found terms */
    static size_t   cap  = 0;      /* current capacity of 'data'  */
    static idx_t    len  = 0;      /* number of terms stored      */
    static seq_t    cur  = 1;      /* next integer to test         */

    /* Extend the table until we have at least 'n' terms. */
    while (len < n) {
        if (has_more_pits_than_peaks(cur)) {
            /* Grow array if necessary. */
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
                if (!tmp) return 0;     /* out of memory – unlikely in tests */
                data = tmp;
                cap  = new_cap;
            }
            data[len++] = cur;
        }
        ++cur;
    }

    return data[n - 1];
}
