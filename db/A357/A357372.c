/*
 * A357372  Square array A(n, k), n, k > 0, read by antidiagonals; A(n, k) is
 *          the numerator of 1/n + 1/k.
 * https://oeis.org/A357372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 772, u16 n <= 9586, u32 n <= 9586, u64 n <= 9586,
 *            u128 n <= 9586, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357372: a(n) is the nth element of the antidiagonal‐ordered square array  
   A(i,j) = (i + j) / gcd(i + j, i * j). */ 

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that n lies on the diagonal where i + j = s. */
    idx_t s = 2;      /* current diagonal sum i + j */
    idx_t r = n;      /* remaining position to locate (1‑based within this diagonal) */

    while (r > s - 1)   /* each diagonal has s - 1 entries */
    {
        r -= (s - 1);
        ++s;
    }

    /* r is now 1‑based index inside the diagonal: i = r, j = s - r. */
    idx_t i = r;
    idx_t j = s - r;

    /* Compute numerator: (i + j) / gcd(i + j, i * j). */
    seq_t sum  = (seq_t)s;              /* i + j == s */
    seq_t prod = (seq_t)i * (seq_t)j;    /* i * j */
    seq_t g    = gcd(sum, prod);        /* use provided helper */
    return sum / g;
}
