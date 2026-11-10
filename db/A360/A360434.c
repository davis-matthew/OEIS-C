/*
 * A360434  a(n) is the greatest number k not yet in the sequence such that
 *          A022290(n) = A022290(k).
 * https://oeis.org/A360434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   group the numbers k by v = A022290(k) (binary masks whose
 *            Fibonacci weights 1,2,3,5,8,... sum to v); within each group
 *            listed increasingly, a(k) is the mirror element (j-th <->
 *            (m+1-j)-th). Computed by enumeration
 * Range:     u8 n <= 170, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 84 terms through n = 9715 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A360434_v(idx_t n)
{
    seq_t fib[70];
    seq_t res = 0;
    int i, bit = 0;
    fib[0] = 0;
    fib[1] = 1;
    for (i = 2; i < 70; ++i) fib[i] = fib[i - 1] + fib[i - 2];
    while (n != 0) {
        if (n & 1) res += fib[bit + 2];
        n >>= 1;
        ++bit;
    }
    return res;
}

static seq_t A360434_f[130];
static seq_t A360434_pre[130];
static seq_t *A360434_out;
static long long A360434_cnt;

static void A360434_rec(int j, seq_t m, seq_t mask)
{
    if (m == 0) {
        if (A360434_out) A360434_out[A360434_cnt] = mask;
        A360434_cnt++;
        return;
    }
    if (j < 0) return;
    if (m > A360434_pre[j]) return;
    A360434_rec(j - 1, m, mask);
    if (A360434_f[j] <= m)
        A360434_rec(j - 1, m - A360434_f[j], mask | (((seq_t)1) << j));
}

seq_t a(idx_t n)
{
    seq_t v = A360434_v(n);
    seq_t nn = (seq_t)n;
    int K = 0, j;
    seq_t result = nn;
    long long idx = -1, i;

    if (v >= 1) {
        A360434_f[0] = 1; K = 1;
        if (v >= 2) {
            A360434_f[1] = 2; K = 2;
            while (K < 125 && A360434_f[K - 1] <= v - A360434_f[K - 2]) {
                A360434_f[K] = A360434_f[K - 1] + A360434_f[K - 2];
                K++;
            }
        }
    }
    for (j = 0; j < K; ++j)
        A360434_pre[j] = (j == 0) ? A360434_f[0] : A360434_pre[j - 1] + A360434_f[j];

    A360434_out = 0;
    A360434_cnt = 0;
    A360434_rec(K - 1, v, (seq_t)0);
    {
        long long total = A360434_cnt;
        seq_t *arr = mem_new(seq_t, (size_t)total);
        A360434_out = arr;
        A360434_cnt = 0;
        A360434_rec(K - 1, v, (seq_t)0);
        for (i = 0; i < total; ++i) {
            if (arr[i] == nn) { idx = i; break; }
        }
        if (idx >= 0) result = arr[total - 1 - idx];
        mem_free(arr);
        A360434_out = 0;
    }
    return result;
}
