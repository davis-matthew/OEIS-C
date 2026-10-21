/*
 * A259437  a(n) = Sum_{k=0..n} p(k)^n, where p(k) is the partition function
 *          A000041.
 * https://oeis.org/A259437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(k)^n, where p(k) is the partition function
 *            A000041
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses Euler's pentagonal theorem to compute partition numbers,
   then sums their n‑th powers via the provided seq_pow function. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_pow.h"

/* Cache of partition numbers p(0..m). */
static seq_t *partitions = NULL;
static idx_t partitions_len = 0;   /* current number of stored elements */

/* Ensure that the cache contains partition numbers up to n. */
static void compute_partitions_up_to(idx_t n)
{
    if (n + 1 <= partitions_len) return;   /* already computed */

    /* Allocate a larger array.  We allocate exactly n+1 elements. */
    seq_t *new_part = seq_alloc_new_array(n + 1);
    if (!new_part) return;   /* allocation failure – leave the old cache */

    /* Copy old values into the new array. */
    for (idx_t i = 0; i < partitions_len; ++i) {
        new_part[i] = partitions[i];
    }

    /* Free the old array and replace the pointer. */
    if (partitions) free_seq_array(partitions);
    partitions = new_part;

    /* Set base case if we are starting from scratch. */
    if (partitions_len == 0) {
        partitions[0] = 1;   /* p(0) = 1 */
    }

    idx_t old_len = partitions_len;          /* number of values already valid */
    partitions_len = n + 1;                  /* new capacity */

    /* Compute partition numbers from old_len (or 1 if old_len==0) up to n. */
    idx_t start = (old_len == 0) ? 1 : old_len;
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = 0;

        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;          /* further pentagonal numbers exceed i */

            /* sign = (-1)^(k+1) */
            if (k & 1) {
                sum += partitions[i - pent1];
            } else {
                sum -= partitions[i - pent1];
            }

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;        /* pent2 may still be ≤ i */

            if (k & 1) {
                sum += partitions[i - pent2];
            } else {
                sum -= partitions[i - pent2];
            }
        }

        partitions[i] = sum;
    }
}

seq_t a(idx_t n)
{
    compute_partitions_up_to(n);

    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = seq_pow(partitions[k], n);
        total += term;
    }
    return total;
}
