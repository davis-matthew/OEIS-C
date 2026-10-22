/*
 * A300442  Number of binary strict trees of weight n.
 * https://oeis.org/A300442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 51, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300442: Number of binary strict trees of weight n.
   Recurrence:  a(0) = a(1) = a(2) = 1
                a(n) = 1 + Σ_{x+y=n, 0 < x < y < n} a(x)·a(y)   for n ≥ 3   */
#include <stdlib.h>

static seq_t *table = NULL;     /* memoisation table */
static idx_t table_size = 0;    /* current number of computed entries (size of table) */

/* Grow the table so that indices up to n are valid and computed. */
static void ensure_capacity(idx_t n)
{
    if (n < table_size) return;     /* already ready */

    /* Allocate more space.  New size is n+1 (to include index n). */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(table, (size_t)new_size * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    table = tmp;

    /* Compute all missing entries from old table_size up to new_size-1. */
    for (idx_t i = table_size; i < new_size; ++i)
    {
        if (i <= 2) {
            table[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)1;          /* the "+1" term of the recurrence */
            idx_t half = (i - 1) / 2;      /* largest x with x < i-x */
            for (idx_t x = 1; x <= half; ++x)
            {
                idx_t y = i - x;           /* y > x automatically */
                sum += table[x] * table[y];
            }
            table[i] = sum;
        }
    }

    table_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return table[n];
}
