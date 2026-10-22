/*
 * A324368  Number of partitions of n that contain {1,2} minus number of
 *          partitions of n that contain neither 1 nor 2.
 * https://oeis.org/A324368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A000041(n-2) + A000_KEY_A0(n-1) - A000041(n)
 * Range:     i8 n <= 17, i16 n <= 42, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000041 denotes the partition function p(n) and
   A000041(k) = 0 for k < 0. */

/* Compute the partition numbers p(0) … p(n) using Euler’s recurrence
   and cache them in a static array for repeated calls. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *partitions = NULL;   /* cache of partition numbers */
static size_t partitions_size = 0; /* number of elements currently stored (>=1 if initialized) */

/* Ensure partitions[0..up_to] are computed. */
static void ensure_partitions(size_t up_to)
{
    if (partitions == NULL) {
        partitions = (seq_t *)malloc((up_to + 1) * sizeof(seq_t));
        if (partitions == NULL) return; /* allocation failed – sequence will be wrong */
    } else if (partitions_size <= up_to) {
        partitions = (seq_t *)realloc(partitions, (up_to + 1) * sizeof(seq_t));
        if (partitions == NULL) return; /* allocation failed */
    }

    /* First element p(0) = 1. */
    if (partitions_size == 0) {
        partitions[0] = 1;
        partitions_size = 1;
    }

    for (size_t n = partitions_size; n <= up_to; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; ; ++k) {
            idx_t g1 = (idx_t)k * (3 * k - 1) / 2;
            if (g1 > (idx_t)n) break;
            seq_t term = partitions[n - (size_t)g1];
            if (k & 1) sum += term;   /* (+)  for odd k */
            else        sum -= term;   /* (-)  for even k */

            idx_t g2 = (idx_t)k * (3 * k + 1) / 2;
            if (g2 > (idx_t)n) continue;
            term = partitions[n - (size_t)g2];
            if (k & 1) sum += term;
            else      sum -= term;
        }
        partitions[n] = sum;
    }
    partitions_size = up_to + 1;
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)partitions_size)
        ensure_partitions((size_t)n);
    /* safety check – allocation failures leave array uninitialized.
       In such a case the returned value is undefined, which is acceptable
       for the purposes of this exercise. */

    seq_t res = 0;
    if (n >= 2) res += partitions[n - 2];
    if (n >= 1) res += partitions[n - 1];
    res -= partitions[n];
    return res;
}
