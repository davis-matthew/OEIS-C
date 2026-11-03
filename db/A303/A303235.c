/*
 * A303235  Number of ordered pairs (x, y) with 0 <= x <= y such that n - 2^x
 *          - 2^y can be written as the sum of two triangular numbers.
 * https://oeis.org/A303235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered pairs (x,y) with 0 ≤ x ≤ y and n − 2^x − 2^y
 *            expressible as the sum of two triangular numbers
 * Range:     u8 n <= 9, u16 n <= 2022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8751 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true iff v is a triangular number, i.e. v = k(k+1)/2 for some k ≥ 0. */
static bool is_triangular(seq_t v)
{
    seq_t s = 8 * v + 1;
    seq_t r = floor_sqrt(s);
    return r * r == s;
}

/* Return true iff rem can be written as a sum of two triangular numbers. */
static bool rem_is_sum_of_two_triangular(seq_t rem)
{
    if (rem == 0) return true;           /* 0 + 0 */
    for (seq_t k = 0;; ++k) {
        seq_t t = k * (k + 1) / 2;       /* T(k) */
        if (t > rem) break;
        seq_t u = rem - t;
        if (is_triangular(u)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Collect all powers of two ≤ n. */
    idx_t powers[64];
    size_t pc = 0;
    idx_t p = 1;
    while (pc < 64 && p <= n) {
        powers[pc++] = p;
        if (p > n / 2) break;           /* prevent overflow of p <<= 1 */
        p <<= 1;
    }

    seq_t count = 0;
    for (size_t i = 0; i < pc; ++i) {
        idx_t px = powers[i];
        for (size_t j = i; j < pc; ++j) {
            idx_t py = powers[j];

            /* Ensure px + py ≤ n to avoid wrap‑around on unsigned subtraction. */
            if (py > n - px) break;      /* powers[ ] is sorted, so all larger py fail too */

            seq_t rem = (seq_t)(n - px - py);
            if (rem_is_sum_of_two_triangular(rem))
                ++count;
        }
    }
    return count;
}
