/*
 * A331854  a(n) is the number of distinct values obtained by partitioning the
 *          binary representation of n into consecutive blocks, and then
 *          applying the bitwise OR operator to the numbers represented by the
 *          blocks.
 * https://oeis.org/A331854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331854: a(n) is the number of distinct values obtained
 * by partitioning the binary representation of n into consecutive
 * blocks and applying the bitwise OR to the numeric values of the
 * blocks.  The function enumerates all 2^(k-1) partitions (k = bit‑
 * length of n) and counts unique OR results.  All arithmetic is
 * performed with idx_t (uint64_t) so the routine works with any
 * seq_t implementation. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Local quicksort for idx_t values.  The maximum number of splits
 * for the problem is 2^(k-1) with k <= 14 (n up to 10000),
 * so at most 8192 elements. */
static void quicksort_idx_t(idx_t *arr, size_t left, size_t right)
{
    while (left < right) {
        idx_t pivot = arr[left + (right - left) / 2];
        size_t i = left, j = right;
        while (i <= j) {
            while (arr[i] < pivot) ++i;
            while (arr[j] > pivot) --j;
            if (i <= j) {
                idx_t tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                ++i;
                if (j > 0) --j;
            }
        }
        if (j < left) break;   /* prevents underflow when j == 0 */
        if (i - left < right - i) {
            if (left < j)
                quicksort_idx_t(arr, left, j);
            left = i;
        } else {
            if (i < right)
                quicksort_idx_t(arr, i, right);
            right = j;
        }
    }
}

seq_t a(idx_t n)
{
    /* Special case n == 0 – its binary string is "0".  Only one
     * partition, OR value 0, so the answer is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Determine bit length k of n (most significant 1 position
     * plus one).  We know n > 0 here. */
    int msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        ++msb;
        tmp >>= 1;
    }
    int k = msb + 1;           /* k >= 1 */

    /* Number of possible partitions: 2^(k-1).  For the problem
     * constraints k <= 14, so splits <= 8192. */
    size_t splits = (size_t)1 << (k - 1);

    /* A small fixed array is enough for all test cases. */
    static idx_t values[1 << 13];   /* 8192 entries */
    size_t count = 0;

    for (size_t mask = 0; mask < splits; ++mask) {
        idx_t cur = 0;
        idx_t res = 0;
        for (int p = k - 1; p >= 0; --p) {
            cur = (cur << 1) | ((n >> p) & 1);
            if (p > 0 && ((mask >> (p - 1)) & 1u))
            {
                res |= cur;
                cur = 0;
            }
        }
        res |= cur;          /* last block */
        values[count++] = res;
    }

    /* Sort the produced OR values. */
    quicksort_idx_t(values, 0, count - 1);

    /* Count distinct values in the sorted array. */
    seq_t distinct = 1;            /* at least one value present */
    for (size_t i = 1; i < count; ++i)
        if (values[i] != values[i - 1])
            ++distinct;

    return distinct;
}
