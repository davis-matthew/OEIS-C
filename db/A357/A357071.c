/*
 * A357071  Number of partitions of n into at most 3 distinct positive
 *          triangular numbers.
 * https://oeis.org/A357071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 3 distinct
 *            positive triangular numbers. It is implemented by enumerating
 *            all unordered subsets of size 1, 2, or 3 from the list of
 *            triangular numbers not exceeding n, and counting those whose sum
 *            equals n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The empty partition counts for n = 0. */
    if (n == 0)
        return 1;

    /* Count how many triangular numbers are ≤ n. */
    idx_t t = 1;
    size_t tri_cnt = 0;
    while (1) {
        seq_t tri = (seq_t)t * (t + 1) / 2;
        if (tri > (seq_t)n)
            break;
        ++tri_cnt;
        ++t;
    }

    /* Reserve storage for the triangular numbers. */
    seq_t *tri = (seq_t *)malloc(tri_cnt * sizeof(seq_t));
    if (!tri)
        return 0;          /* allocation failure */

    /* Fill the array. */
    t = 1;
    size_t idx = 0;
    while (idx < tri_cnt) {
        tri[idx++] = (seq_t)t * (t + 1) / 2;
        ++t;
    }

    /* Boolean lookup for triangular numbers up to n. */
    bool *is_tri = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_tri) {
        free(tri);
        return 0;
    }
    for (idx = 0; idx < tri_cnt; ++idx)
        if ((size_t)tri[idx] <= n)
            is_tri[(size_t)tri[idx]] = true;

    seq_t result = 0;
    seq_t sn = (seq_t)n;

    /* 1‑term partitions. */
    if (is_tri[(size_t)sn])
        ++result;

    /* 2‑term partitions (unordered, distinct). */
    {
        idx_t left = 0, right = tri_cnt - 1;
        while (left < right) {
            seq_t sum = tri[left] + tri[right];
            if (sum == sn) {
                ++result;
                ++left;
                --right;
            } else if (sum < sn) {
                ++left;
            } else {
                --right;
            }
        }
    }

    /* 3‑term partitions (unordered, distinct). */
    for (idx_t i = 0; i + 2 < (idx_t)tri_cnt; ++i) {
        seq_t ti = tri[i];
        for (idx_t j = i + 1; j + 1 < (idx_t)tri_cnt; ++j) {
            seq_t tj = tri[j];
            seq_t sum2 = ti + tj;
            if (sum2 > sn)
                break;          /* further j will only increase sum2 */
            seq_t rem = sn - sum2;
            if (rem <= tj)     /* must be strictly larger than tj */
                continue;
            if ((size_t)rem <= n && is_tri[(size_t)rem])
                ++result;
        }
    }

    free(tri);
    free(is_tri);
    return result;
}
