/*
 * A305441  Indices k such that A305355(k) = 0.
 * https://oeis.org/A305441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A305355(k) = 0, where A305355 =
 *            coefficients of Product_{k>=1} (1 - x^(k*(3*k-1)/2)); computed
 *            by a series expansion up to k = 10316
 * Range:     u8 n <= 99, u16 n <= 227, u32 n <= 227, u64 n <= 227,
 *            u128 n <= 227, big any n
 * Validated: standard, pass, 82 terms through n = 226 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

#define A305441_LIM 10316

static int A305441_cnt = -1;
static unsigned A305441_idx[A305441_LIM + 1];

static void A305441_build(void)
{
    static long long c[A305441_LIM + 1];
    size_t N = A305441_LIM;
    for (size_t i = 0; i <= N; ++i) c[i] = 0;
    c[0] = 1;
    for (size_t k = 1; ; ++k) {
        size_t e = k * (3 * k - 1) / 2;
        if (e > N) break;
        for (size_t i = N; i >= e; --i) {
            c[i] -= c[i - e];
            if (i == e) break;
        }
    }
    int m = 0;
    for (size_t i = 1; i <= N; ++i)
        if (c[i] == 0) A305441_idx[m++] = (unsigned)i;
    A305441_cnt = m;
}

seq_t a(idx_t n)
{
    if (A305441_cnt < 0) A305441_build();
    if (n < 1 || n > (idx_t)A305441_cnt) return 0;
    return (seq_t)A305441_idx[n - 1];
}
