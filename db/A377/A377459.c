/*
 * A377459  Square array T(n,k) read by ascending antidiagonals: number of
 *          steps for a certain Euclidean-style algorithm (see below) to find
 *          the GCD of n and k.
 * https://oeis.org/A377459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from OEIS A377459: number of subtraction steps to reach the GCD
 * for the pair (i,j) obtained by reading the square array T(i,j) along
 * ascending antidiagonals.  The mapping from index n to (i,j) is:
 *
 *   s = smallest integer with (s-1)*s/2 >= n   (s is the diagonal number, s >= 2)
 *   prev = (s-2)*(s-1)/2                       (elements before this diagonal)
 *   pos  = n - prev - 1                        (position within the diagonal,
 *                                               0-indexed, decreasing i)
 *   i    = (s-1) - pos
 *   j    = s - i
 *
 * The function then performs the Euclidean‐difference algorithm:
 *
 *   while (i != j) {
 *       k = |i - j|;
 *       i = j;   j = k;   steps++;
 *   }
 *
 * and returns the number of steps.  This is the desired a(n).
 */

#include <stdbool.h>

static seq_t euclid_steps(seq_t a, seq_t b)
{
    seq_t steps = 0;
    while (a != b) {
        seq_t diff = (a > b) ? (a - b) : (b - a);
        a = b;
        b = diff;
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    /* special case: n == 0 is outside of OEIS offset but safe */
    if (n == 0)
        return 0;

    /* find diagonal s such that (s-1)*s/2 >= n */
    idx_t s = 2;           /* minimal diagonal */
    idx_t cumulative = 1;  /* elements up to diagonal s */
    while (n > cumulative) {
        ++s;
        cumulative += (s - 1);
    }

    /* elements before this diagonal */
    idx_t prev = cumulative - (s - 1);

    /* position within the diagonal (0-based, decreasing i) */
    idx_t pos = n - prev - 1;

    seq_t i = (s - 1) - pos;
    seq_t j = s - i;

    return euclid_steps(i, j);
}
