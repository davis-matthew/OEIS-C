/*
 * A346617  Irregular triangle T(n,m) read by rows (n >= 1, 1 <= m <=
 *          Max(A001221([1..n]))): T(n,m) = number of integers in [1,n] with m
 *          distinct prime factors.
 * https://oeis.org/A346617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of the irregular triangle T(n,m) = number
 *            of integers i in [1,n] having exactly m distinct prime factors,
 *            read row by row. The sequence is built incrementally by counting
 *            prime factors up to n and storing the cumulative counts for each
 *            m in that row
 * Range:     u8 n <= 1678, u16 n <= 7831, u32 n <= 7831, u64 n <= 7831,
 *            u128 n <= 7831, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n) {
    /* static state between calls */
    static seq_t *seq = NULL;          /* flattened triangle values          */
    static size_t seq_len = 0;         /* current number of stored values    */
    static size_t seq_cap = 0;         /* allocated capacity for seq          */
    static idx_t current_row = 1;      /* next row to process                */
    static int current_max_m = 0;      /* maximum m seen so far (column count)  */
    static seq_t *counts = NULL;       /* cumulative counts for each m (T(row,m)) */
    static size_t counts_cap = 0;      /* capacity of counts array            */

    /* expand the triangle up to the requested index */
    while (seq_len < n) {
        /* Handle row 1 separately (count of numbers with 0 primes) */
        if (current_row == 1) {
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)0;   /* T(1,0) = 0 in the flattened sequence */
            ++current_row;
            continue;
        }

        /* Determine number of distinct prime factors of current_row */
        int m = count_distinct_primes((uint64_t)current_row);

        /* Ensure counts array has room for m */
        if ((size_t)m >= counts_cap) {
            size_t new_cap = counts_cap ? counts_cap * 2 : ((m + 1) * 2);
            while (new_cap <= (size_t)m) new_cap <<= 1;
            counts = (seq_t *)realloc(counts, new_cap * sizeof(seq_t));
            /* initialize new slots to zero */
            for (size_t i = counts_cap; i < new_cap; ++i)
                counts[i] = (seq_t)0;
            counts_cap = new_cap;
        }

        /* Update cumulative count for this row */
        counts[m] = counts[m] + (seq_t)1;
        if (m > current_max_m) current_max_m = m;

        /* Ensure seq has room for this whole row */
        if (seq_len + (size_t)current_max_m > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            while (new_cap < seq_len + (size_t)current_max_m)
                new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* Append the current row to the flattened sequence */
        for (int mi = 1; mi <= current_max_m; ++mi)
            seq[seq_len++] = counts[mi];

        ++current_row;
    }

    return seq[n - 1];
}
