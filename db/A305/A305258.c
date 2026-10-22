/*
 * A305258  List of y-coordinates of a point moving in a smooth
 *          counterclockwise spiral rotated by Pi/4.
 * https://oeis.org/A305258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305258: y‑coordinates of a counter‑clockwise spiral
   rotated by Pi/4.  The following implementation uses the
   equivalent piecewise description:

   Let k be the largest integer such that k*(k+1)/2 ≤ n;
   let d = n − k*(k+1)/2;
   if d = 0       → a(n) = 0
   else
       m   = (k+1)/2   (integer division);
       v   = (d ≤ m) ? d : k+1-d;
       a(n)= (k odd) ?  v : −v
   This reproduces a(n) for all n ≥ 0. */

#include <stdint.h>

/* Helper: find largest k such that k*(k+1)/2 ≤ n
   Works for all 0 ≤ n < 2^64. */
static idx_t
max_k(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponentially increase high until it exceeds n
       (using 128‑bit to avoid overflow). */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search between low and high.  The loop
       terminates with low == the desired k. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Handle the trivial cases quickly.  The general
       algorithm below also works for n = 0,1. */
    if (n <= 1)
        return 0;

    idx_t k = max_k(n);                    /* k satisfies k(k+1)/2 ≤ n */
    unsigned __int128 t = ((unsigned __int128)k * (k + 1)) / 2; /* triangular number */
    idx_t d = n - (idx_t)t;               /* distance from the start of the segment */

    if (d == 0)
        return 0;

    idx_t mid = (k + 1) / 2;
    idx_t val = (d <= mid) ? d : (k + 1 - d);

    /* Determine sign: positive for odd k, negative for even k. */
    int sign = (k & 1) ? 1 : -1;

    return (seq_t)((seq_t)sign * (seq_t)val);
}
