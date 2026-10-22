/*
 * A319468  Number of partitions of n into exactly two nonzero decimal
 *          palindromes.
 * https://oeis.org/A319468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = number of unordered pairs of nonzero decimal palindromes
 *            summing to n
 * Range:     u8 n <= 1112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* Cache of palindromes up to the largest n seen so far */
    static idx_t max_n = 0;          /* largest n processed */
    static seq_t *pal = NULL;        /* array of palindromes in ascending order */
    static size_t pal_cnt = 0;       /* number of stored palindromes   */
    static size_t pal_cap = 0;       /* allocated capacity of pal      */

    /* n <= 1 cannot be partitioned into two positive palindromes */
    if (n <= 1)
        return 0;

    /* Grow the palindrome list if needed */
    if (n > max_n) {
        /* Ensure enough space for the new palindromes we'll add */
        if (pal_cap < (size_t)(n + 10)) {
            size_t new_cap = (size_t)(n + 10);
            seq_t *tmp = (seq_t *)realloc(pal, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failure – return 0 as safe default */
            pal = tmp;
            pal_cap = new_cap;
        }

        /* Generate palindromes in the new interval (max_n, n] */
        for (idx_t v = max_n + 1; v <= n; ++v) {
            if (is_palindrome((seq_t)v, 10UL))
                pal[pal_cnt++] = (seq_t)v;
        }
        max_n = n;
    }

    seq_t result = 0;

    /* For each palindrome p <= n/2, check if n-p is also a palindrome */
    for (size_t i = 0; i < pal_cnt; ++i) {
        seq_t p = pal[i];
        if ((idx_t)p > n / 2)
            break;
        idx_t diff = n - (idx_t)p;          /* n - p is non‑negative */
        seq_t q = (seq_t)diff;

        /* Binary search for q in the sorted palindrome array */
        size_t lo = 0, hi = pal_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            seq_t mv = pal[mid];
            if (mv < q)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < pal_cnt && pal[lo] == q)
            ++result;
    }

    return result;
}
