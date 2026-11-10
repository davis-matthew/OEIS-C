/*
 * A258007  First differences of A258203.
 * https://oeis.org/A258007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258203(n+1) - A258203(n); A258203 found by search: k ->
 *            floor(tan(k)) orbit reaches 1 staying positive
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9265 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned long long *A258203_arr = NULL;
static size_t A258203_cap = 0;
static size_t A258203_len = 0;
static long long A258203_next = 1;

static long long A258203_floor_tan(long long x)
{
    double v = tan((double)x);
    return (long long)floor(v);
}

static bool A258203_qualifies(long long m)
{
    long long k = m;
    long long min_k = k;
    while (1) {
        if (k <= 0) return false;
        long long f = A258203_floor_tan(k);
        if (f == k) return (k > 0);
        if (f < min_k) min_k = f;
        if (f == 1) return (min_k > 0);
        k = f;
    }
}

static unsigned long long A258203_a(size_t n)
{
    while (A258203_len < n) {
        while (!A258203_qualifies(A258203_next)) ++A258203_next;
        if (A258203_len >= A258203_cap) {
            size_t nc = A258203_cap ? A258203_cap * 2 : 64;
            unsigned long long *t = (unsigned long long *)realloc(A258203_arr, nc * sizeof(unsigned long long));
            if (!t) abort();
            A258203_arr = t;
            A258203_cap = nc;
        }
        A258203_arr[A258203_len++] = (unsigned long long)A258203_next;
        ++A258203_next;
    }
    return A258203_arr[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned long long hi = A258203_a((size_t)n + 1);
    unsigned long long lo = A258203_a((size_t)n);
    return (seq_t)(hi - lo);
}
