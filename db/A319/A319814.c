/*
 * A319814  Number of partitions of n into exactly four positive triangular
 *          numbers.
 * https://oeis.org/A319814
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into exactly four
 *            positive triangular numbers (OEIS A319814)
 * Range:     u8 n <= 1731, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Number of partitions of n into exactly four positive triangular numbers. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Determine the maximal k with k(k+1)/2 <= n and collect all triangular numbers. */
    size_t k = 1;
    while ((unsigned long long)k * (k + 1) / 2 <= n) ++k;
    size_t max_k = k - 1;
    size_t tri_cnt = max_k;

    idx_t *tri = (idx_t *)malloc(tri_cnt * sizeof(idx_t));
    if (!tri) return 0;
    for (size_t i = 1; i <= max_k; ++i)
        tri[i - 1] = (idx_t)i * (i + 1) / 2;

    /* Boolean array to test whether a number <= n is triangular. */
    size_t sz = (size_t)n + 1;
    unsigned char *is_tri = (unsigned char *)calloc(sz, 1);
    if (!is_tri) {
        free(tri);
        return 0;
    }
    for (size_t i = 0; i < tri_cnt; ++i)
        is_tri[(size_t)tri[i]] = 1;

    /* Count unordered quadruples (t1 <= t2 <= t3 <= t4). */
    seq_t count = 0;
    idx_t N = n;

    for (size_t i = 0; i < tri_cnt; ++i) {
        idx_t t1 = tri[i];
        if (4 * t1 > N) break;

        for (size_t j = i; j < tri_cnt; ++j) {
            idx_t t2 = tri[j];
            if (t1 + 3 * t2 > N) break;

            for (size_t k = j; k < tri_cnt; ++k) {
                idx_t t3 = tri[k];
                idx_t sum3 = t1 + t2 + t3;
                if (sum3 + t3 > N) break;

                idx_t t4 = N - sum3;
                if (t4 < t3) continue;
                if (t4 <= n && is_tri[(size_t)t4]) ++count;
            }
        }
    }

    free(is_tri);
    free(tri);
    return count;
}
