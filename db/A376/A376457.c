/*
 * A376457  Let s(x) be the Maclaurin series for cos(x); then a(n) is the
 *          index k for which (k+1)-st partial sum of s(2*n*Pi) is least among
 *          all partial sums.
 * https://oeis.org/A376457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: For n ≥ 1 let x = 2πn.  Let k be the largest integer with
 *          (2k)(2k‑1) ≤ x² = 4π² n².  Then a(n) is the index of the
 *          minimal partial sum and is given by  
 *                a(n) = k            if k is odd,
 *                a(n) = k – 1        if k is even.
 *
 * This uses a fixed‑point integer approximation of 4π²:
 *      C = floor(4π² × 10¹²) = 39478417604357
 *      SCALE = 10¹².
 * The inequality is tested as
 *      (2k)(2k‑1) × SCALE ≤ C × n²,
 * which can be performed entirely with integer arithmetic.
 * A binary search in [0, 4n] yields the exact k.
 */

#include <stdint.h>

#define SCALE 1000000000000ULL        /* 10¹² fixed‑point scaling      */
#define C    39478417604357ULL       /* floor(4π² × SCALE)           */

seq_t a(idx_t n)
{
    if (n == 0)                      /* defined only for n ≥ 1       */
        return 0;

    __uint128_t right = (__uint128_t)C * (__uint128_t)n * (__uint128_t)n;

    uint64_t low = 0, high = 4 * n, best = 0;

    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);

        /* left side: (2k)(2k‑1) × SCALE  */
        __uint128_t left = (__uint128_t)(2 * mid) * (2 * mid - 1);
        left *= SCALE;

        if (left <= right)
        {
            best = mid;
            low  = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    uint64_t result = (best & 1ULL) ? best : (best - 1ULL);
    return (seq_t)result;
}
