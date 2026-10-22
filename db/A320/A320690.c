/*
 * A320690  Number of partitions of n with up to three distinct kinds of 1.
 * https://oeis.org/A320690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 2*p(n-1) - 2*p(n-3) - p(n-4) where p(k) is the
 *            ordinary partition number of k
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 131, u64 n <= 430,
 *            u128 n <= 1501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of partition numbers p(0..cap-1). */
static seq_t *part = NULL;
static idx_t  cap  = 0;   /* number of computed partitions */

/* Extend the cache so that p(0..new_cap-1) are available. */
static void extend_part(idx_t new_cap)
{
    if (new_cap <= cap) return;                 /* already computed */

    seq_t *new_part = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_part) exit(1);

    /* Copy previously computed values. */
    for (idx_t i = 0; i < cap; ++i) new_part[i] = part[i];

    /* Initialize p(0) if this is the first allocation. */
    if (cap == 0) new_part[0] = (seq_t)1;

    /* Compute partition numbers from the next index upwards
       using the generalized pentagonal number theorem. */
    idx_t start = (cap > 0) ? cap : 1;
    for (idx_t i = start; i < new_cap; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if ((k & 1) == 1) sum += new_part[i - pent1];
            else              sum -= new_part[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if ((k & 1) == 1) sum += new_part[i - pent2];
            else              sum -= new_part[i - pent2];
        }
        new_part[i] = sum;
    }

    free(part);
    part = new_part;
    cap  = new_cap;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Ensure partition numbers up to n are available. */
    extend_part(n + 1);

    seq_t res = part[n];
    if (n >= 1) {
        seq_t tmp = part[n-1];
        tmp += tmp;          /* multiply by 2 */
        res += tmp;
    }
    if (n >= 3) {
        seq_t tmp = part[n-3];
        tmp += tmp;          /* multiply by 2 */
        res -= tmp;
    }
    if (n >= 4) res -= part[n-4];
    return res;
}
