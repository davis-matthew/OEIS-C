/*
 * A338687  Number of ways to write n as x^4 + y^2 + floor(z^2/7), where x,y,z
 *          are integers with x >= 0, y >= 1 and z >= 2.
 * https://oeis.org/A338687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence counts the representations
 * n = x^4 + y^2 + floor(z^2/7),  x≥0, y≥1, z≥2.
 * The implementation does not use a closed formula; it builds the counts
 * of the possible floor(z^2/7) values up to n and then enumerates all
 * admissible x and y.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

static seq_t *z_counts = NULL;   /* counts of floor(z^2/7) values   */
static idx_t  z_counts_max = 0;  /* maximum n for which z_counts is valid */

/* Ensure that z_counts[w] is defined for all w <= n. */
static void
ensure_z_counts(idx_t n)
{
    if (n <= z_counts_max)
        return;

    /* free any previous allocation */
    if (z_counts)
        free(z_counts);

    /* allocate a fresh zero‑initialised array of length n+1 */
    z_counts = alloc_seq((size_t)n + 1);
    if (!z_counts)
        return;   /* allocation failed */

    /* initialise to zero (alloc_seq may already do this, but be safe) */
    for (idx_t i = 0; i <= n; ++i)
        z_counts[i] = (seq_t)0;

    /* count values of floor(z^2/7) that occur for z >= 2 */
    idx_t bound = 7 * n + 6;
    idx_t max_z = 0;
    while ((max_z + 1) * (max_z + 1) <= bound)
        ++max_z;

    for (idx_t z = 2; z <= max_z; ++z) {
        idx_t w = (z * z) / 7;      /* floor division */
        if (w <= n)
            z_counts[w] += (seq_t)1;
    }

    z_counts_max = n;
}

/* Main function requested by the exercise. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_z_counts(n);
    if (!z_counts)
        return (seq_t)0;           /* allocation failed */

    seq_t total = (seq_t)0;

    /* enumerate x ≥ 0 such that x^4 ≤ n */
    for (idx_t x = 0; ; ++x) {
        idx_t x4 = x * x * x * x;
        if (x4 > n)
            break;
        idx_t rem1 = n - x4;

        /* enumerate y ≥ 1 such that y^2 ≤ rem1 */
        for (idx_t y = 1; ; ++y) {
            idx_t y2 = y * y;
            if (y2 > rem1)
                break;
            idx_t w = rem1 - y2;   /* required floor(z^2/7) value */
            total += z_counts[w];
        }
    }

    return total;
}
