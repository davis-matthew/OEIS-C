/*
 * A287792  Positions of 1 in A287790; complement of A287791.
 * https://oeis.org/A287792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287790. Each Fibonacci-word
 *            bit f at position p expands to 001 (f=0) or 110 (f=1), so the
 *            code binary-searches p using prefix counts of 1s in the
 *            Fibonacci word
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9840 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A287792_K 92

static uint64_t A287792_len[A287792_K];
static uint64_t A287792_ones[A287792_K];
static int A287792_init = 0;

static void A287792_setup(void)
{
    if (A287792_init) return;
    A287792_len[0] = 1; A287792_ones[0] = 0;
    A287792_len[1] = 2; A287792_ones[1] = 1;
    for (int k = 2; k < A287792_K; k++) {
        A287792_len[k] = A287792_len[k-1] + A287792_len[k-2];
        A287792_ones[k] = A287792_ones[k-1] + A287792_ones[k-2];
    }
    A287792_init = 1;
}

/* number of 1s among the first m symbols of the Fibonacci word */
static uint64_t A287792_prefix_ones(uint64_t m)
{
    uint64_t total = 0;
    while (m > 0) {
        int k = 0;
        while (k + 1 < A287792_K && A287792_len[k + 1] <= m) k++;
        total += A287792_ones[k];
        m -= A287792_len[k];
    }
    return total;
}

static uint64_t A287792_count(uint64_t p)
{
    return p + A287792_prefix_ones(p);
}

seq_t a(idx_t n)
{
    A287792_setup();
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t lo = 1, hi = N;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A287792_count(mid) >= N) hi = mid;
        else lo = mid + 1;
    }
    uint64_t p = lo;
    uint64_t before = A287792_count(p - 1);
    uint64_t j = N - before;
    uint64_t f = A287792_prefix_ones(p) - A287792_prefix_ones(p - 1);
    seq_t base = (seq_t)p * 3;
    if (f == 0) return base;
    return base - (seq_t)3 + (seq_t)j;
}
