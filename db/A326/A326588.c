/*
 * A326588  Sum of all the parts in the partitions of n into 10 parts.
 * https://oeis.org/A326588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A026816(n) A026816(n): number of partitions of n into
 *            exactly 10 parts So a(n) is the sum of all parts in all 10‑part
 *            partitions of n
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Static storage for partitions p_k(n): number of partitions of n into exactly k parts.
   We build only up to k = 10. */
static seq_t *part[11] = { NULL };
static size_t part_cap = 0;   /* current capacity (number of indices stored) */

/* Ensure that the arrays have capacity for indices 0..n (inclusive). */
static void _ensure_cap(idx_t n)
{
    if ((size_t)n < part_cap) return;   /* already sufficient */

    size_t new_cap = (part_cap ? part_cap * 2 : 1);
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

    /* Allocate new arrays for each k and copy old data. */
    for (int k = 0; k <= 10; ++k) {
        seq_t *new_arr = alloc_seq_array(new_cap);
        if (!new_arr) abort();        /* allocation failed */

        /* Copy existing data */
        for (size_t i = 0; i < part_cap; ++i)
            new_arr[i] = part[k][i];

        /* Free old array if it existed */
        if (part[k]) free_seq_array(part[k]);

        part[k] = new_arr;
    }

    /* Fill in values for new indices */
    for (size_t idx = part_cap; idx < new_cap; ++idx) {
        /* k = 0: only one partition of 0 */
        part[0][idx] = (idx == 0) ? 1 : 0;

        /* k >= 1 */
        for (int k = 1; k <= 10; ++k) {
            if (idx < (size_t)k) {
                part[k][idx] = 0;
            } else {
                /* recurrence: q_k(n) = q_k(n-k) + q_{k-1}(n-1) */
                seq_t t1 = part[k][idx - k];
                seq_t t2 = part[k - 1][idx - 1];
                part[k][idx] = t1 + t2;
            }
        }
    }

    part_cap = new_cap;
}

/* External function producing the sequence value. */
seq_t a(idx_t n)
{
    _ensure_cap(n);
    /* a(n) = n * p_10(n) */
    seq_t res = (seq_t)n * part[10][n];
    return res;
}
