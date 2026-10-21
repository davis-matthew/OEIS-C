/*
 * A287255  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks are <= five.
 * https://oeis.org/A287255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A287216(n,5) – number of set partitions of [n] such that
 *            the absolute difference between the least elements of
 *            consecutive blocks is at most 5. For n≤6 this coincides with the
 *            Bell numbers. The algorithm processes the elements 1…n one by
 *            one, keeping a DP state (k,s) where k is the number...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* There are 7 possible values of s: 0…6 (6 means s>5). */
    const int S = 7;
    /* Allocate two DP rows of size (n+1) × S. */
    size_t rows   = (size_t)(n + 2);      /* one extra for safety */
    size_t stride = (size_t)S;
    size_t sz     = rows * stride;

    /* In C++ calloc returns void*, so cast explicitly. */
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !nxt) {
        /* Allocation failure – return 0 as a safe default. */
        free(cur);
        free(nxt);
        return 0;
    }

    /* After placing element 1, we have one block with its least at 1.
       The distance to itself is 0. */
    cur[1 * stride + 0] = 1;

    /* Process elements 2 … n (pos from 1 to n-1). */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* Clear nxt entries that will be used in the next iteration. */
        size_t max_k_next = pos + 1;         /* at most pos+1 blocks */
        for (idx_t k = 1; k <= max_k_next; ++k) {
            for (int s = 0; s < S; ++s) {
                nxt[k * stride + s] = 0;
            }
        }

        /* Iterate over current states. */
        for (idx_t k = 1; k <= pos; ++k) {
            for (int s = 0; s < S; ++s) {
                seq_t ways = cur[k * stride + s];
                if (!ways) continue;

                /* Option 1: add element to the current block. */
                int new_s = (s == 6) ? 6 : (s + 1);
                nxt[k * stride + new_s] += ways * (seq_t)k;

                /* Option 2: start a new block if allowed. */
                if (s <= 4) {   /* ensures new distance ≤5 */
                    nxt[(k + 1) * stride + 0] += ways;
                }
            }
        }

        /* Swap cur and nxt for the next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all states after processing element n. */
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        for (int s = 0; s < S; ++s) {
            total += cur[k * stride + s];
        }
    }

    free(cur);
    free(nxt);
    return total;
}
