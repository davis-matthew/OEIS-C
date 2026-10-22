/*
 * A400216  Scaled x-coordinates of the vertices of a Boerdijk-Coxeter helix
 *          starting at a tetrahedron with vertices (-1,-1,-1), (1, -1, 1),
 *          (-1, 1, 1), and (1, 1, -1).
 * https://oeis.org/A400216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   For n = 0..6 the terms are -1, -1, 1, 1, 5, 31, 83. For n ≥ 7
 *            the recurrence holds: a(n) = 2*a(n-1) + 6*a(n-2) + 18*a(n-3) -
 *            81*a(n-4)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an iterative linear‑recurrence algorithm. */

#include <stdint.h> /* for uint64_t, if needed by idx_t or seq_t */

seq_t a(idx_t n)
{
    /* initial terms (a(0) … a(6)) */
    static const seq_t init[7] = {-1, -1, 1, 1, 5, 31, 83};
    if (n < 7)
        return init[n];

    /* t0 = a(n-4), t1 = a(n-3), t2 = a(n-2), t3 = a(n-1) */
    seq_t t0 = init[3];   /* a(3) */
    seq_t t1 = init[4];   /* a(4) */
    seq_t t2 = init[5];   /* a(5) */
    seq_t t3 = init[6];   /* a(6) */

    seq_t next;
    for (idx_t i = 7; i <= n; ++i)
    {
        next = 2 * t3 + 6 * t2 + 18 * t1 - 81 * t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
