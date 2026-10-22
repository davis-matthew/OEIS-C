/*
 * A359034  a(n+1) is the sum of the number of terms in all groups of
 *          contiguous terms that add up to a(n); a(1)=1.
 * https://oeis.org/A359034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 699, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1. For n >= 1,
 * a(n+1) = sum of the lengths of all contiguous subarrays of
 * a(1), …, a(n) whose elements sum to a(n).  All terms are positive,
 * so a sliding‑window algorithm yields an O(n^2) solution.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised sequence */
    static seq_t *seq = NULL;   /* array of already computed terms */
    static size_t cap = 0;      /* capacity of the array */
    static idx_t len = 0;       /* number of terms currently stored (0 means none) */

    /* initialise a(1) on first call */
    if (len == 0) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)1;      /* a(1) = 1 */
        len = 1;
    }

    /* if requested term is already computed, return it */
    if (n <= len)
        return seq[n - 1];

    /* grow the static array if needed for new terms */
    if (n > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < (size_t)n)
            newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute any missing terms up to n */
    for (idx_t cur = len + 1; cur <= n; ++cur) {
        /* target is a(cur-1) */
        seq_t target = seq[cur - 2];

        /* sliding‑window scan over a[1..cur-1] (array indices 0..cur-2) */
        seq_t total = 0;
        idx_t left = 0;
        seq_t sum = 0;

        for (idx_t right = 0; right < cur - 1; ++right) {
            sum += seq[right];

            /* shrink from left until sum <= target */
            while (sum > target && left <= right) {
                sum -= seq[left];
                ++left;
            }

            if (sum == target) {
                total += (seq_t)(right - left + 1);
            }
        }

        seq[cur - 1] = total;
        len = cur;
    }

    return seq[n - 1];
}
