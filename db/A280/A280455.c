/*
 * A280455  Number of ways to write n as x*(3x-1)/2 + y*(3y+1)/2 + p(z), where
 *          x,y,z are nonnegative integers with z > 0, and p(.) is the
 *          partition function given by A000041.
 * https://oeis.org/A280455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A280455:  a(n) = #{ (x,y,z) | x,y,z≥0 , z>0,
 *   n = x*(3x-1)/2 + y*(3y+1)/2 + p(z) },
 *   where p(z) is the partition function A000041.  */

/* Includes required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Static cache for partition numbers p(z), using 0‑based indexing:
 *  p[0] = 1, p[1] = 1, p[2] = 2, ... */
static seq_t *partitions = NULL; /* array of p(z) */
static idx_t   part_count = 0;   /* number of elements stored (≥1 after init) */
static idx_t   part_cap   = 0;   /* capacity of the array */

/* Forward declarations */
static seq_t compute_partition(idx_t z);
static void  ensure_partitions_upto_value(idx_t n);
static bool  is_pentagonal(const seq_t t);

/* Compute the partition number p(z) using the pentagonal number theorem.
 * Assumes partitions[0..z-1] are already computed. */
static seq_t compute_partition(idx_t z)
{
    if (z == 0) return 1;

    /* Use signed 64‑bit accumulator for intermediate sums. */
    int64_t sum = 0;

    for (idx_t i = 1; ; ++i) {
        idx_t pent1 = i * ((idx_t)3 * i - 1) / 2;
        if (pent1 > z) break;
        int sign = (i & 1) ? 1 : -1;
        sum += sign * (int64_t)partitions[z - pent1];

        idx_t pent2 = i * ((idx_t)3 * i + 1) / 2;
        if (pent2 > z) continue;
        sum += sign * (int64_t)partitions[z - pent2];
    }
    return (seq_t)sum;
}

/* Ensure that partitions[0..z] are computed for all z such that
 * p(z) ≤ n.  The array is expanded lazily. */
static void ensure_partitions_upto_value(idx_t n)
{
    if (partitions == NULL) {
        part_cap   = 16;
        partitions = (seq_t *)malloc(part_cap * sizeof(seq_t));
        partitions[0] = 1;  /* p(0) = 1 */
        part_count   = 1;
    }

    while (partitions[part_count - 1] <= n) {
        idx_t z = part_count;                 /* compute next p(z) */
        seq_t val = compute_partition(z);

        if (part_count == part_cap) {          /* grow the array */
            part_cap *= 2;
            partitions = (seq_t *)realloc(partitions,
                                           part_cap * sizeof(seq_t));
        }
        partitions[part_count] = val;
        part_count++;
    }
}

/* Test whether a non‑negative integer t is of the form y*(3y+1)/2
 * for some integer y ≥ 0.  Uses integer square root. */
static bool is_pentagonal(const seq_t t)
{
    seq_t discrim = 1 + 24 * t;                     /* 1 + 24t */
    seq_t s = floor_sqrt(discrim);
    return ((s * s == discrim) && ((s - 1) % 6 == 0));
}

/* Main sequence function.  Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    /* Step 1: determine all z with p(z) ≤ n. */
    ensure_partitions_upto_value(n);

    /* Find the largest z such that p(z) ≤ n. */
    idx_t zmax;
    zmax = part_count - 1;                 /* may be > n */
    while (zmax > 0 && partitions[zmax] > n) zmax--;

    seq_t count = 0;                       /* result accumulator */

    /* Step 2: iterate over all admissible z. */
    for (idx_t z = 1; z <= zmax; ++z) {
        seq_t rem = n - partitions[z];     /* remaining after partition part */

        /* For each z, count all x,y ≥ 0 satisfying
         * x*(3x-1)/2 + y*(3y+1)/2 = rem. */
        for (idx_t x = 0; ; ++x) {
            /* Compute g1(x) = x*(3x-1)/2 */
            seq_t g1 = (seq_t)x * (((seq_t)3 * x) - 1) / 2;
            if (g1 > rem) break;

            seq_t t = rem - g1;            /* residual to be represented by y */
            if (is_pentagonal(t))
                ++count;
        }
    }
    return count;
}
