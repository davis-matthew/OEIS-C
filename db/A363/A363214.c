/*
 * A363214  Number of partitions of n with rank 5 (the rank of a partition is
 *          the largest part minus the number of parts).
 * https://oeis.org/A363214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k>=1} (-1)^{k-1} * ( p(n-(5k + k(3k-1)/2)) * -
 *            p(n-(5k + k(3k+1)/2)) ) where p(m) is the number of partitions
 *            of m (p(0)=1)
 * Range:     u8 n <= 30, u16 n <= 64, u32 n <= 165, u64 n <= 489,
 *            u128 n <= 1608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of partition numbers p(i) for i = 0 .. current_size-1. */
static seq_t *part_cache = NULL;   /* array indexed by m */
static idx_t  part_capacity = 0;   /* allocated size */
static idx_t  part_size = 0;       /* highest index computed (>=0) */

/* Ensure partition numbers are available up to 'need' (inclusive). */
static void ensure_partition(idx_t need)
{
    if (need <= part_size)
        return; /* already available */

    /* Allocate or grow array. */
    size_t new_cap = part_capacity ? part_capacity : 1;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(part_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    part_cache = tmp;
    part_capacity = new_cap;

    /* Compute new partition numbers. */
    if (part_size == 0) {
        /* p(0) = 1 */
        part_cache[0] = (seq_t)1;
        part_size = 0;
    }
    for (idx_t m = part_size + 1; m <= need; ++m) {
        seq_t pos = 0, neg = 0; /* positive and negative sums for recurrence */
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m)
                break;
            idx_t g2 = k * (3 * k + 1) / 2;
            int sign = (k & 1) ? 1 : -1; /* (-1)^(k-1) */
            if (sign > 0) {
                pos += part_cache[m - g1];
                if (g2 <= m)
                    pos += part_cache[m - g2];
            } else {
                neg += part_cache[m - g1];
                if (g2 <= m)
                    neg += part_cache[m - g2];
            }
        }
        part_cache[m] = pos - neg; /* pos >= neg for partition numbers */
    }
    part_size = need;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined for n=0, but harmless */

    /* Ensure partition numbers up to n are ready. */
    ensure_partition(n);

    seq_t pos = 0, neg = 0;
    for (idx_t k = 1; ; ++k) {
        /* Generalized pentagonal numbers */
        idx_t pent1 = k * (3 * k - 1) / 2;
        idx_t pent2 = k * (3 * k + 1) / 2;
        /* Shifts for the rank-5 generating function */
        idx_t shift1 = 5 * k + pent1;
        idx_t shift2 = 5 * k + pent2;

        if (n < shift1 && n < shift2)
            break; /* no further contributions */

        int sign = (k & 1) ? 1 : -1; /* (-1)^(k-1) */

        if (n >= shift1) {
            if (sign > 0)
                pos += part_cache[n - shift1];
            else
                neg += part_cache[n - shift1];
        }
        if (n >= shift2) {
            if (sign > 0)
                neg += part_cache[n - shift2];
            else
                pos += part_cache[n - shift2];
        }
    }

    return pos - neg; /* pos >= neg for all n */
}
