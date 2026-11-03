/*
 * A295148  Solution of the complementary equation a(n) = a(n-1) + 2*a(n-2) +
 *          b(n-1), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + b(n-1) with a(0)=1, a(1)=3, b(0)=2,
 *            b(1)=4 and (a(n)) and (b(n)) are complementary increasing
 *            sequences
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline bool exists_in_a(const seq_t *arr, idx_t size, seq_t val)
{
    idx_t lo = 0, hi = size - 1;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate arrays to hold a[0..n] and b[0..n] */
    seq_t *a_arr = mem_new(seq_t, (size_t)(n + 1));
    seq_t *b_arr = mem_new(seq_t, (size_t)(n + 1));
    if (!a_arr || !b_arr) {
        if (a_arr) mem_free(a_arr);
        if (b_arr) mem_free(b_arr);
        return (seq_t)0; /* allocation failed */
    }

    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)3;
    b_arr[0] = (seq_t)2;
    b_arr[1] = (seq_t)4;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-2) + b(i-1) */
        a_arr[i] = a_arr[i-1] + (seq_t)2 * a_arr[i-2] + b_arr[i-1];

        /* Find b(i): smallest integer > b(i-1) that is not in a[0..i] */
        seq_t cand = b_arr[i-1] + (seq_t)1;
        while (exists_in_a(a_arr, i + 1, cand))
            cand = cand + (seq_t)1;
        b_arr[i] = cand;
    }

    seq_t result = a_arr[n];
    mem_free(a_arr);
    mem_free(b_arr);
    return result;
}
