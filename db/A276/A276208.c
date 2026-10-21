/*
 * A276208  Position of n! in the joint ranking of {2^h} and {k!}, where h >=
 *          0, k >= 0.
 * https://oeis.org/A276208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(log2(n!)) - (n ≥ 2 ? 1 : 0)
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the rank of n! in the sorted, duplicate‑free list of
 * all powers of 2 and all factorials.  The rank equals the count of
 * distinct numbers ≤ n!:
 *   count = (#factorials ≤ n!) + (#powers of 2 ≤ n!) – (#overlaps)
 * where the overlaps are {1, 2}.  Hence the closed form above.
 */

#include <stdlib.h>
#include <math.h>

/* Static cache storing floor(log2(k!)) for k = 0 … current_max */
static idx_t *floor_log2_arr = NULL;
static idx_t floor_arr_len = 0;
static long double cumulative_log = 0.0L;

/* Ensure that floor_log2_arr[n] is defined for all n ≤ target */
static void ensure_floor_log2(idx_t target)
{
    if (floor_arr_len == 0) {
        floor_log2_arr = (idx_t*)malloc(sizeof(idx_t));
        if (!floor_log2_arr) exit(1);
        floor_log2_arr[0] = 0;          /* log2(0!) = log2(1) = 0 */
        floor_arr_len = 1;
    }
    if (target + 1 <= floor_arr_len) return;

    size_t newlen = (size_t)(target + 1);
    idx_t *tmp = (idx_t*)realloc(floor_log2_arr, newlen * sizeof(idx_t));
    if (!tmp) exit(1);
    floor_log2_arr = tmp;

    for (idx_t i = floor_arr_len; i <= target; ++i) {
        cumulative_log += log2l((long double)i);
        floor_log2_arr[i] = (idx_t)(floorl(cumulative_log + 1e-12L));
    }
    floor_arr_len = newlen;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) is undefined, but harmless */

    ensure_floor_log2(n);

    idx_t sub = (n >= 2) ? 1 : 0;
    seq_t result = (seq_t)n + (seq_t)floor_log2_arr[n] - (seq_t)sub;
    return result;
}
