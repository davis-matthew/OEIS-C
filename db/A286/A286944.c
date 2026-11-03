/*
 * A286944  Number of ways to write n as x^2 + 15*y^2 + z*(3z+1)/2, where x
 *          and y are nonnegative integers and z is an integer.
 * https://oeis.org/A286944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9867 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the number of representations of n as
   n = x^2 + 15*y^2 + z*(3z+1)/2 with x,y ≥ 0 and z ∈ ℤ.
   The algorithm enumerates all z such that z(3z+1)/2 ≤ n.
   For each such z it reduces the problem to counting solutions
   of x^2 + 15*y^2 = r where r = n - z(3z+1)/2.
   It then enumerates all y with 15*y^2 ≤ r and checks whether
   r - 15*y^2 is a perfect square.  This direct brute‑force
   search is fast enough for n up to about 10 000 (≈2000 operations).  */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* --- iterate over non‑negative z --- */
    for (int64_t z = 0; ; ++z) {
        seq_t T = ((seq_t)z * ((seq_t)3 * z + 1)) / 2;
        if (T > n) break;
        seq_t r = n - T;

        for (int64_t y = 0; ; ++y) {
            seq_t t15 = ((seq_t)15 * y * y);
            if (t15 > r) break;
            seq_t s = r - t15;
            seq_t x = floor_sqrt(s);
            if (x * x == s) ++count;
        }
    }

    /* --- iterate over negative z --- */
    for (int64_t z = -1; ; --z) {
        seq_t T = ((seq_t)z * ((seq_t)3 * z + 1)) / 2;
        if (T > n) break;
        seq_t r = n - T;

        for (int64_t y = 0; ; ++y) {
            seq_t t15 = ((seq_t)15 * y * y);
            if (t15 > r) break;
            seq_t s = r - t15;
            seq_t x = floor_sqrt(s);
            if (x * x == s) ++count;
        }
    }

    return count;
}
