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
 * Helpers:   min_uv_sq, modinv_idx, seq_alloc_new_array, seq_free_array
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
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/min_uv_sq.h"
#include "../utils/modinv_idx.h"

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
