/*
 * A308137  Number of (undirected) Hamiltonian paths on the n-prism graph.
 * https://oeis.org/A308137
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3 + (-1)^n + 2*n^2) / 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For smaller n the value is
       unspecified; we return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Convert to a signed 64-bit type for intermediate arithmetic.
       For n up to about 1e5 the intermediate values fit comfortably
       in a 64‑bit signed integer. */
    long long nn = (long long)n;

    /* Compute using the explicit formula.
       2*n*n may overflow a 32‑bit int, so cast to long long first. */
    long long term = 2 * nn * nn;                /* 2*n^2   */
    term += 3;                                   /* +3      */
    term += ((n & 1) ? -1 : 1);                  /* +(-1)^n */
    term *= nn;                                  /* *n      */
    term /= 2;                                   /* /2      */

    return (seq_t)term;
}
