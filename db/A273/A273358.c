/*
 * A273358  Numbers n such that the decimal number concat(3,n) is a square.
 * https://oeis.org/A273358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 3337, u64 n <= 11067,
 *            u128 n <= 11067, big any n
 * Validated: standard, pass, 58 terms through n = 8753 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation: Enumerate all squares in the ranges
 *  [3·10^d, 4·10^d − 1] for digit length d = 1,2,…
 *  For each square s = x² subtract low = 3·10^d.
 *  Keep only those n = s − low that have exactly d decimal digits
 *  (i.e., n ≥ 10^(d‑1)).  Count such n until the requested index n is
 *  reached, then return the corresponding n.  The procedure works for
 *  any integer‐size type seq_t that supports the required arithmetic
 *  operations.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    if (n == 0)          /* Undefined for index 0 – caller must use n ≥ 1 */
        return (seq_t)0;

    idx_t count = 0;     /* number of valid terms found so far */

    for (idx_t d = 1; ; ++d)
    {
        seq_t pow10_d   = pow10(d);          /* 10^d   */
        seq_t low       = (seq_t)3 * pow10_d;/* 3·10^d  */
        seq_t high      = (seq_t)4 * pow10_d - 1; /* 4·10^d-1 */

        seq_t lo = floor_sqrt(low);
        if (lo * lo < low)   /* ceil sqrt if needed */
            ++lo;
        seq_t hi = floor_sqrt(high);         /* floor sqrt */

        for (seq_t x = lo; x <= hi; ++x)
        {
            seq_t sq          = x * x;
            seq_t n_candidate = sq - low;     /* n = 3… concatenated with n */

            /* Check that n has exactly d digits: n ≥ 10^(d-1) */
            seq_t min_dig = pow10(d - 1);
            if (n_candidate >= min_dig)
            {
                ++count;
                if (count == n)
                    return n_candidate;
            }
        }
        /* Continue with next digit length */
    }
}
