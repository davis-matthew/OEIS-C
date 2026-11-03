/*
 * A345434  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of u^2+v^2.
 * https://oeis.org/A345434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{x=1..n}\sum_{y=1..n} min_uv_sq(x/gcd(x,y),
 *            y/gcd(x,y)) where min_uv_sq(a,b) is the minimal value of u^2+v^2
 *            over integers u,v with u*a + v*b = 1 (and (x,y) = (g*a, g*b))
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sequence naively by iterating over all
   ordered pairs (a,b) up to n, computing the minimal norm solution of
   a*u + b*v = 1 using modular inverses, and summing as described.
   It uses the provided utilities for arbitrary‑precision arithmetic
   when available. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* modular inverse of a modulo m (gcd(a,m)=1) */
static idx_t modinv_idx(idx_t a, idx_t m)
{
    int64_t r0 = a, r1 = m;
    int64_t s0 = 1, s1 = 0;
    int64_t t0 = 0, t1 = 1;
    while (r1) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;
        int64_t s2 = s0 - q * s1;
        int64_t t2 = t0 - q * t1;
        r0 = r1; r1 = r2;
        s0 = s1; s1 = s2;
        t0 = t1; t1 = t2;
    }
    /* r0 == 1, s0 is the inverse of a modulo m */
    int64_t inv = s0 % (int64_t)m;
    if (inv < 0) inv += (int64_t)m;
    return (idx_t)inv;
}

/* minimal u^2+v^2 for coprime a,b */
static seq_t min_uv_sq(idx_t a, idx_t b)
{
    idx_t inv = modinv_idx(a, b);          /* 0 <= inv < b */
    int64_t u = (int64_t)inv;
    if (u > (int64_t)b / 2) u -= (int64_t)b;   /* bring u to [-b/2, b/2] */
    int64_t v = (1 - u * (int64_t)a) / (int64_t)b;

    int64_t absu = u >= 0 ? u : -u;
    int64_t absv = v >= 0 ? v : -v;
    seq_t squ = (seq_t)absu * (seq_t)absu;
    seq_t sqv = (seq_t)absv * (seq_t)absv;
    return squ + sqv;
}

/* Euclidean gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1 */
    idx_t N = n;
    /* allocate array that will hold S(m) for 0..N (prefix sums) */
    seq_t *summax = seq_alloc_new_array((size_t)(N + 1));
    for (idx_t i = 0; i <= N; ++i) summax[i] = 0;

    /* accumulate contributions for each coprime pair (a,b) */
    for (idx_t a1 = 1; a1 <= N; ++a1) {
        for (idx_t b1 = 1; b1 <= N; ++b1) {
            if (gcd_idx(a1, b1) != 1) continue;
            idx_t m = a1 > b1 ? a1 : b1;          /* maximum of a1,b1 */
            summax[m] += min_uv_sq(a1, b1);
        }
    }

    /* prefix sums: summax[m] becomes S(m) = sum_{max(a,b)<=m} min_uv_sq */
    for (idx_t m = 1; m <= N; ++m) {
        summax[m] += summax[m - 1];
    }

    /* compute final result: a(n) = sum_{m=1..n} S(m) * (⌊n/m⌋ - ⌊n/(m+1)⌋) */
    seq_t result = 0;
    for (idx_t m = 1; m <= N; ++m) {
        idx_t cnt = (n / m) - (n / (m + 1));
        result += summax[m] * (seq_t)cnt;
    }

    seq_free_array(summax);
    return result;
}
