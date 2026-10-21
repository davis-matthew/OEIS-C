/*
 * A299473  a(n) = 3*p(n), where p(n) is the number of partitions of n.
 * https://oeis.org/A299473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * p(n) where p(n) is the partition number of n
 * Range:     u8 n <= 12, u16 n <= 37, u32 n <= 117, u64 n <= 398,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

/* Cached partition numbers p(0)…p(max) */
static seq_t *partitions  = NULL;  /* pointer to sequence array   */
static size_t partitions_cap   = 0; /* current allocated size      */
static idx_t  partitions_max   = 0; /* highest n for which p[n] known */

/* Compute a(n) = 3 * p(n) */
seq_t a(idx_t n)
{
    /* Make sure the buffer can hold an entry for index n. */
    ensure_cache(n, &partitions, &partitions_cap);

    /* The partition table must contain p(0)=1.  Initialise it only once. */
    if (partitions[0] == (seq_t)0)
        partitions[0] = (seq_t)1;

    /* Compute partition numbers up to n if needed. */
    if (n > partitions_max) {
        for (idx_t k = partitions_max + 1; k <= n; ++k) {
            seq_t sumPos = 0;
            seq_t sumNeg = 0;

            /* Euler’s recurrence using generalized pentagonal numbers. */
            for (idx_t m = 1; ; ++m) {
                idx_t g1 = m * (3 * m - 1) / 2;
                if (g1 > k) break;                    /* no more terms */

                /* sign alternates with period 2: +, +, -, -, +, +, … */
                if (m & 1)
                    sumPos += partitions[k - g1];
                else
                    sumNeg += partitions[k - g1];

                idx_t g2 = m * (3 * m + 1) / 2;
                if (g2 > k) continue;
                if (m & 1)
                    sumPos += partitions[k - g2];
                else
                    sumNeg += partitions[k - g2];
            }

            partitions[k] = sumPos - sumNeg;  /* guaranteed non‑negative */
        }
        partitions_max = n;
    }

    /* a(n) = 3 * p(n) */
    seq_t ans = partitions[n];
    ans *= (seq_t)3;
    return ans;
}
