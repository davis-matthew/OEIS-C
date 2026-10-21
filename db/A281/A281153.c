/*
 * A281153  Least number k such that Sum_{j=k..k+n-1}{j^2} =
 *          Sum_{j=k+n..t}{j^2}, for some t >= k+n.
 * https://oeis.org/A281153
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281153:
 * a(n) = the least integer k such that
 *   Σ_{j=k}^{k+n-1} j^2 = Σ_{j=k+n}^{t} j^2   for some t ≥ k+n.
 * The implementation follows the definition directly, using only
 * integer arithmetic and the helper for the sum of the first x squares.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* -------------- helper: sum of squares 1..x ---------------------------------- */
static inline seq_t sumsq(idx_t x)
{
    seq_t a = (seq_t)x;
    seq_t b = a + 1;          /* x + 1 */
    seq_t c = a * 2 + 1;       /* 2x + 1 */
    return a * b * c / 6;
}

/* -------------- binary search for t --------------------------------------- */
static idx_t find_t(seq_t R, idx_t min_t)
{
    if (min_t == 0) min_t = 1;
    idx_t low  = min_t;
    idx_t high = min_t;

    /* grow high until S(high) >= R or the maximum idx_t is reached */
    while (sumsq(high) < R) {
        if (high > (idx_t)(UINT64_MAX >> 1))
            break;      /* cannot double any more without overflow            */
        high <<= 1;
    }

    /* if even the largest reachable high is still too small, no solution */
    if (sumsq(high) < R) return 0;

    /* standard binary search for exact equality */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t val = sumsq(mid);
        if (val == R) return mid;
        if (val < R)  low = mid + 1;
        else          high = mid - 1;
    }
    return 0;          /* no integer t satisfies the equation */
}

/* -------------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* offset 2 */

    idx_t k = 1;
    while (true) {
        /* RHS of equation: 2*S(k+n-1) - S(k-1)  must equal S(t)            */
        seq_t s1 = sumsq(k + n - 1);
        seq_t s0 = sumsq(k - 1);
        seq_t R  = 2 * s1 - s0;

        /* Quick check with the most common t = k + 2*(n-1)                  */
        idx_t t0 = k + 2 * (n - 1);
        if (t0 >= k + n && sumsq(t0) == R)
            return (seq_t)k;

        /* If R is already less than the minimal possible S(t), skip */
        if (R >= sumsq(k + n)) {
            idx_t t = find_t(R, k + n);
            if (t != 0) return (seq_t)k;
        }
        k++;
    }
}
