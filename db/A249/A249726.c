/*
 * A249726  Numbers n such that there is a multiple of 36 on row n of Pascal's
 *          triangle with property that it is also the least multiple of 4 and
 *          the least multiple of 9 on the same row.
 * https://oeis.org/A249726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 4547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_p_borrows, find_min_prime_distance, is_good_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249726: compute by checking rows of Pascal's triangle for equality of
   the minimal distances to the nearest zero modulo 4 and modulo 9. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/count_p_borrows.h"
#include "../utils/find_min_prime_distance.h"
#include "../utils/is_good_row.h"

/* External types are provided by the build. */
typedef uint64_t idx_t;

/* Cache for the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_len   = 0;        /* number of terms stored */
static idx_t  next_row  = 1;        /* next row to test */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* Extend cache until we have n terms. */
    while (seq_len < n) {
        idx_t row = next_row++;
        if (is_good_row(row)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* out‑of‑memory */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)row;
        }
    }
    return seq_cache[n - 1];
}
