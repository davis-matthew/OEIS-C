/*
 * A281726  Triangular array read by rows: T(n,k) is the number of elements in
 *          an n X k matrix that will be assigned the same value whether the
 *          integers from 1 to n*k are assigned to elements in row-major order
 *          or column-major order.
 * https://oeis.org/A281726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + gcd(r-1, c-1) where r, c are defined by * n =
 *            r*(r-1)/2 + c, 1 ≤ c ≤ r * (the sequence is the flattening of
 *            the triangular array T(n,k) = 1 + gcd(n-1,k-1) by rows)
 * Range:     u8 n <= 9379, u16 n <= 9379, u32 n <= 9379, u64 n <= 9379,
 *            u128 n <= 9379, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 safely */
    if (n == 0) return 0;

    /* Find row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n.
       Since n is at most the order of 10⁴ for the intended tests,
       a simple O(√n) loop is fast enough and avoids any floating‑point
       or heavy library calls. */
    idx_t r = 1;
    idx_t thistotal = 1;          /* r*(r+1)/2 for current r */
    while (thistotal < n) {
        ++r;
        thistotal = (r * (r + 1)) / 2;
    }

    /* First index of this row (1‑based) */
    idx_t first_in_row = ((r - 1) * r) / 2;
    /* Column index (1‑based) */
    idx_t c = n - first_in_row;

    /* Compute the value: 1 + gcd(r-1, c-1) */
    seq_t g = gcd((seq_t)(r - 1), (seq_t)(c - 1));
    return 1 + g;
}
