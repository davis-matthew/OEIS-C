/*
 * A285721  Square array read by antidiagonals: A(n,k) = number of steps in
 *          simple Euclidean algorithm for gcd(n,k) to reach the termination
 *          test n=k, read by antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3),
 *          A(2,2), A(3,1), etc.
 * https://oeis.org/A285721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the number of subtraction steps in the simple Euclidean
 *            algorithm to reduce the pair (i,j) corresponding to the n‑th
 *            antidiagonal entry, where (i,j) = A(n,k). The pair is obtained
 *            by d = smallest integer with d*(d+1)/2 >= n offset = n -
 *            (d-1)*d/2 i = offset j = d - offset + 1 and the recu...
 * Range:     u8 n <= 9744, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute the number of subtraction steps for the Euclidean algorithm on
   the pair (a,b).  The result fits in seq_t if it fits in the output type. */
static seq_t euclid_steps(idx_t a, idx_t b)
{
    seq_t steps = 0;
    while (a != b)
    {
        if (a > b)
        {
            idx_t q   = a / b;
            idx_t rem = a % b;
            if (rem == 0)
            {
                /* The last subtraction would bring a to 0; we stop at equality
                   after performing q-1 subtractions. */
                steps += (seq_t)(q - 1);
                break;
            }
            steps += (seq_t)q;
            a = rem;
        }
        else
        {
            idx_t q   = b / a;
            idx_t rem = b % a;
            if (rem == 0)
            {
                steps += (seq_t)(q - 1);
                break;
            }
            steps += (seq_t)q;
            b = rem;
        }
    }
    return steps;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1, but defensive */

    /* Find diagonal d: the smallest integer with d*(d+1)/2 >= n. */
    long double t = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t d = (idx_t)((t - 1.0L) / 2.0L);

    /* Correct possible rounding from the square root. */
    while ((__int128)d * (d + 1) / 2 < n)
        d++;
    while (d > 0 && (__int128)(d - 1) * d / 2 >= n)
        d--;

    /* Offset within the diagonal. */
    idx_t prev   = (idx_t)((__int128)(d - 1) * d / 2);   /* S(d-1) */
    idx_t offset = n - prev;                           /* 1 .. d */
    idx_t i = offset;
    idx_t j = d - offset + 1;

    return euclid_steps(i, j);
}
