/*
 * A334653  Number of integer partitions of n with at least two parts, each
 *          greater than 1, at least two kinds of parts and all with the same
 *          multiplicity.
 * https://oeis.org/A334653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d∣n} (p_distinct_gt1(d) – 1) where p_distinct_gt1(d)
 *            is the number of partitions of d into distinct parts larger than
 *            1. This is equivalent to the OEIS definition a(n) = 1 + Σ_{d∣n}
 *            (A025147(d) – 1)
 * Range:     u8 n <= 33, u16 n <= 85, u32 n <= 249, u64 n <= 813,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *part_counts = NULL;   /* Number of partitions of i into
                                       distinct parts >1 (any size). */
static idx_t part_counts_size = 0;  /* Current allocated size of part_counts. */

static void compute_part_counts(idx_t up_to)
{
    idx_t new_size = up_to + 1;
    size_t len = (size_t)new_size;
    seq_t *cnt = (seq_t *)malloc(len * sizeof(seq_t));
    if (!cnt)
        return;           /* Allocation failure – sequence will fail silently. */

    for (idx_t i = 0; i < new_size; ++i)
        cnt[i] = 0;
    cnt[0] = 1;           /* Empty partition for sum 0. */

    for (idx_t part = 2; part <= up_to; ++part) {
        for (idx_t sum = up_to; sum >= part; --sum) {
            cnt[sum] += cnt[sum - part];
        }
    }

    free(part_counts);
    part_counts = cnt;
    part_counts_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n >= part_counts_size)
        compute_part_counts(n);

    seq_t result = 0;

    /* Enumerate all divisors of n. */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* Add f(d1) = part_counts[d1] – 1 (for d1 > 1). */
            if (d1 > 1) {
                seq_t val = part_counts[d1];
                /* part_counts[d1] is at least 1 for d1 > 1. */
                result += val - 1;
            }

            if (d2 != d1 && d2 > 1) {
                seq_t val = part_counts[d2];
                result += val - 1;
            }
        }
    }

    return result;
}
