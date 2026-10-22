/*
 * A355390  Number of ordered pairs of distinct integer partitions of n.
 * https://oeis.org/A355390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) * (p(n) - 1) where p(n) is the number of
 *            unrestricted integer partitions of n. p(0) is defined as 1. The
 *            sequence values above therefore follow. The partition numbers
 *            are computed via Euler's pentagonal theorem using only unsigned
 *            arithmetic in seq_t. The implementation keeps an array...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The following header imports are minimal and only use standard
   facilities; seq_t and idx_t are defined by the build system. */
#include <stddef.h>
#include <stdlib.h>

/* Static storage for partition values. */
static seq_t *partitions = NULL;
static size_t parts_cap = 0;      /* capacity of the allocations (in elements) */
static size_t parts_size = 0;     /* number of computed partition values */

/* Ensure the array has room for indices 0 .. n. */
static void ensure_parts_cap(idx_t n)
{
    size_t need = (size_t)n + 1; /* need indices up to n inclusive */
    if (parts_cap < need) {
        parts_cap = need;
        if (partitions == NULL) {
            partitions = (seq_t *)malloc(parts_cap * sizeof(seq_t));
        } else {
            partitions = (seq_t *)realloc(partitions, parts_cap * sizeof(seq_t));
        }
    }
}

/* Compute partition numbers p(i) for all i between current parts_size
   and n (inclusive).  Uses Euler's generalized pentagonal number formula.
   The recurrence with alternating signs is done in two accumulators
   (pos, neg) to avoid unsigned underflow. */
static void compute_until(idx_t n)
{
    if (parts_size > (size_t)n) return;   /* already computed */

    ensure_parts_cap(n);

    /* Initialize p(0) on first call */
    if (parts_size == 0) {
        partitions[0] = 1;     /* p(0) = 1 */
        parts_size = 1;
    }

    for (idx_t i = (idx_t)parts_size; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1;; ++k) {
            /* Generalized pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = partitions[(size_t)(i - pent1)];
            if (k & 1) pos += term; else neg += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = partitions[(size_t)(i - pent2)];
            if (k & 1) pos += term; else neg += term;
        }
        partitions[(size_t)i] = pos - neg;   /* pos >= neg for all i */
    }
    parts_size = (size_t)n + 1;
}

/* Return a(n) = p(n)*(p(n)-1) for the given n. */
seq_t a(idx_t n)
{
    compute_until(n);
    seq_t p = partitions[(size_t)n];
    return p * (p - 1);
}
