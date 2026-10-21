/*
 * A282093  Larger member of a pair (x,y) which solves x^2+y^2 = z^3 for
 *          positive x, y and z.
 * https://oeis.org/A282093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A282093:
   larger member y of a pair (x,y) with 1 ≤ x ≤ y satisfying x² + y² = z³.
   The implementation tests each integer y for the existence of integers
   x and z fulfilling the equation, accumulating valid y values in ascending
   order.  The sequence is generated lazily, so calls to a() are order‑independent. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

/* global cache of computed terms */
static seq_t *seq_cache = NULL;  /* array of terms */
static size_t  seq_len  = 0;    /* current number of stored terms   */
static size_t  seq_cap  = 0;    /* current allocation capacity      */
static idx_t   next_y   = 1;    /* next Y to test   */

/* store a new term in the cache, expanding capacity if necessary */
static void push_term(seq_t y)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap > 0 ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort execution */
            exit(1);
        }
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = y;
}

/* main routine – returns the n‑th element of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return 0;                    /* sequence is 1‑indexed */
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t y = (seq_t)next_y;
        seq_t y_square = y * y;      /* y² is safe for the ranges needed */

        /* z must satisfy z³ > y² and z³ ≤ 2·y²   */
        seq_t z_start = floor_cube_root(y_square) + 1;
        seq_t z_end   = floor_cube_root((seq_t)((seq_t)2 * y_square));

        bool found = false;
        for (seq_t z = z_start; z <= z_end; ++z) {
            seq_t z_cube = z * z * z;
            seq_t x_square = z_cube - y_square;
            seq_t x = floor_sqrt(x_square);
            if (x * x == x_square && x <= y) {
                found = true;
                break;
            }
        }

        if (found) {
            push_term(y);
        }
        ++next_y;
    }

    return seq_cache[n - 1];
}
