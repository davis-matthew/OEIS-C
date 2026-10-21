/*
 * A246305  Numbers k such that cos(k) > cos(k+1) > cos(k+2) < cos(k+3).
 * https://oeis.org/A246305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k ≥ 1 such that cos(k) > cos(k+1) >
 *            cos(k+2) < cos(k+3). The function finds the term by a linear
 *            search
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Test whether integer k satisfies the required pattern. */
static bool good_k(idx_t k)
{
    double d  = (double)k;
    double c1 = cos(d);
    double c2 = cos(d + 1.0);
    double c3 = cos(d + 2.0);
    double c4 = cos(d + 3.0);
    return (c1 > c2 && c2 > c3 && c3 < c4);
}

/* Return the n‑th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 */

    idx_t count = 0;
    for (idx_t k = 1;; ++k)            /* search indefinitely */
    {
        if (good_k(k))
        {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
