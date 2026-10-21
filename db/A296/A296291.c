/*
 * A296291  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n-1), where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n*b(n-1), a(0)=2, a(1)=3, b(0)=1,
 *            b(1)=4, a(n) and b(n) are increasing complementary sequences
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool contains(const seq_t *arr, size_t len, seq_t value) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == value) return true;
        if (arr[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

#define MAX_N 20000
static seq_t a_arr[MAX_N + 1];
static seq_t b_arr[MAX_N + 1];
static size_t a_len = 0;
static size_t b_len = 0;

seq_t a(idx_t n) {
    if (a_len == 0) {
        a_arr[0] = (seq_t)2;
        a_arr[1] = (seq_t)3;
        a_len = 2;
        b_arr[0] = (seq_t)1;
        b_arr[1] = (seq_t)4;
        b_len = 2;
    }
    if (n < a_len) return a_arr[n];

    for (idx_t i = a_len; i <= n; ++i) {
        seq_t ai = a_arr[i - 1] + a_arr[i - 2] + ((seq_t)i) * b_arr[i - 1];
        a_arr[i] = ai;
        a_len = i + 1;
        seq_t cand = b_arr[i - 1] + 1;
        while (contains(a_arr, a_len, cand) || contains(b_arr, b_len, cand))
            ++cand;
        b_arr[b_len] = cand;
        ++b_len;
    }
    return a_arr[n];
}
