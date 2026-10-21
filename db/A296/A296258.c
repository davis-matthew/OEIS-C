/*
 * A296258  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)^2, where a(0) = 1, a(1) = 3, b(0) = 2, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)^2, a(0) = 1, a(1) = 3, b(0) = 2,
 *            with (a(n)) and (b(n)) the increasing complementary sequences
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* Binary search for x in a sorted array a[0..len-1] */
static int contains_a(const seq_t *a, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] == x) return 1;
        if (a[mid] <  x) lo = mid + 1;
        else              hi = mid;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate sequences up to index n */
    seq_t *a_arr = alloc_seq_array(n + 1);
    seq_t *b_arr = alloc_seq_array(n + 1);
    if (!a_arr || !b_arr) { /* unlikely, but safe */
        seq_free_array(a_arr);
        seq_free_array(b_arr);
        return (seq_t)0;
    }

    /* Initialize known terms */
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)3;
    size_t a_len = 2;                 /* Number of a values stored */
    b_arr[0] = (seq_t)2;
    size_t b_len = 1;                 /* Number of b values stored */

    for (idx_t i = 2; i <= n; ++i) {
        /* Ensure b(i-2) is available via generating needed b values */
        while (b_len < (size_t)(i - 1)) {        /* need b[0 .. i-2] */
            seq_t cand = (b_len == 0) ? (seq_t)1
                                      : b_arr[b_len - 1] + (seq_t)1;
            while (contains_a(a_arr, a_len, cand))
                cand += (seq_t)1;
            b_arr[b_len++] = cand;
        }

        /* Compute a(i) = a(i-1) + a(i-2) + b(i-2)^2 */
        seq_t bi_sq = b_arr[i - 2] * b_arr[i - 2];
        a_arr[i] = a_arr[i - 1] + a_arr[i - 2] + bi_sq;
        ++a_len;
    }

    seq_t result = a_arr[n];
    seq_free_array(a_arr);
    seq_free_array(b_arr);
    return result;
}
