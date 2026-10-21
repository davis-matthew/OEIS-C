/*
 * A270706  Number of ordered ways to write n as x^2*T(x) + y^2 + T(z), where
 *          x, y and z are integers with x nonzero, y positive and z
 *          nonnegative, and T(m) denotes the triangular number m*(m+1)/2.
 * https://oeis.org/A270706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute force enumeration of solutions (x,y,z) with bounds derived from n. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(v)) for any seq_t. Handles overflow safely. */
static seq_t isqrt(seq_t v)
{
    seq_t low = 0, high = v;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        /* mid*mid <= v  <=>  mid <= v/mid  (avoids overflow) */
        if (mid <= v / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* safety */
            high = mid - 1;
        }
    }
    return ans;
}

/* Count the number of integer triples (x,y,z) with a fixed x that satisfy
   x^2*T(x) + y^2 + T(z) == target. y>0, z>=0, T(k)=k(k+1)/2. */
static seq_t countYZ(seq_t target)
{
    seq_t count = 0;
    /* y maximum is floor(sqrt(target)) */
    seq_t yMax = isqrt(target);
    for (seq_t y = 1; y <= yMax; ++y) {
        seq_t ySq = y * y;
        if (ySq > target) break;
        seq_t rem = target - ySq;
        if (rem < 0) break;

        /* Check if rem is triangular: rem = z(z+1)/2 */
        seq_t d = 8 * rem + 1;         /* discriminant */
        seq_t s = isqrt(d);
        if (s == 0) continue;
        seq_t q = d / s;
        if (s == q && d % s == 0 && (s & 1u)) {
            ++count;
        }
    }
    return count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    seq_t total = 0;

    /* Iterate over |x| = 1,2,... ; break when both Apos and Aneg exceed seq_n */
    for (seq_t absx = 1;; ++absx) {
        seq_t x_sq = absx * absx;

        /* Positive x */
        seq_t x_pos = absx;
        seq_t Tpos = x_pos * (x_pos + 1) / 2;
        seq_t Ap = x_sq * Tpos;
        /* Negative x */
        seq_t x_neg = -absx;
        seq_t Tneg = x_neg * (x_neg + 1) / 2;
        seq_t An = x_sq * Tneg;

        if (Ap > seq_n && An > seq_n) break;

        if (Ap <= seq_n) total += countYZ(seq_n - Ap);
        if (An <= seq_n) total += countYZ(seq_n - An);
    }

    return total;
}
