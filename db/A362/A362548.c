/*
 * A362548  Number of partitions of n with at least three parts larger than 1.
 * https://oeis.org/A362548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - ( 1 + [n>=2]*(n-1) + Σ_{b=2}^{⌊n/2⌋} (n-2b+1) )
 *            where p(n) is the partition function. This implements the exact
 *            formula and computes p(n) by Euler's pentagonal recurrence
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Number of partitions of n with at most two parts larger than 1. */
static seq_t count_small_partitions(idx_t n)
{
    seq_t cnt = 1;                     /* all ones (empty partition when n==0) */
    if (n >= 2)
        cnt += (seq_t)(n - 1);         /* one part > 1 */
    if (n >= 4) {
        for (idx_t b = 2; b <= n / 2; ++b) {
            idx_t max_a = n - b;
            if (max_a < b)
                break;
            cnt += (seq_t)(max_a - b + 1);
        }
    }
    return cnt;
}

/* Static array holding partition numbers p[0..max] and the current max index. */
static seq_t *part_arr = NULL;
static idx_t   part_max = 0;

/* Ensure that part_arr is at least size n+1 and contains p[0..n]. */
static void ensure_partitions(idx_t n)
{
    if (n <= part_max)
        return;

    /* Grow the array if necessary. */
    seq_t *new_arr = mem_resize(seq_t, part_arr, (size_t)(n + 1));
    if (!new_arr) {          /* Allocation failed – keep old data unchanged. */
        new_arr = part_arr;
    } else {
        part_arr = new_arr;
    }
    if (!part_arr)
        return;              /* Give up if memory is unavailable. */

    /* Initialize the partition of 0. */
    part_arr[0] = 1;

    /* Compute p[i] for i = part_max+1 .. n using Euler's pentagonal theorem. */
    for (idx_t i = part_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i)
                break;
            seq_t term = part_arr[i - pent1];
            if (k & 1) pos += term; else neg += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i)
                continue;
            term = part_arr[i - pent2];
            if (k & 1) pos += term; else neg += term;
        }
        part_arr[i] = pos - neg;   /* pos ≥ neg, so no underflow */
    }
    part_max = n;
}

/* OEIS function: number of partitions of n with at least three parts > 1. */
seq_t a(idx_t n)
{
    ensure_partitions(n);
    if (!part_arr)           /* allocation failed – return 0 as safe fallback */
        return 0;
    seq_t total = part_arr[(size_t)n];
    seq_t small = count_small_partitions(n);
    return total - small;     /* remaining partitions have ≥3 parts > 1 */
}
