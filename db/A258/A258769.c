/*
 * A258769  a(n) = Number of times the k-th term is equal to k in the modified
 *          Collatz trajectory of n, when counting the initial term n as the
 *          1st term: n, A014682(n), A014682(A014682(n)), ...
 * https://oeis.org/A258769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of indices k such that the k‑th term of the
 *            modified Collatz trajectory of n equals k. Trajectory: T(1)=n,
 *            T(i+1)=A014682(T(i)), where A014682(m) = (m/2) if m even,
 *            ((3m+1)/2) if m odd. Termination when the value reaches 1
 * Range:     u8 n <= 26, u16 n <= 9981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;      /* current term of the trajectory */
    seq_t count = 0;         /* number of fixed points found */
    idx_t pos = 1;           /* current position (1‑based) */

    for (;;)
    {
        if (x == (seq_t)pos)      /* fixed point at this position? */
            count++;

        if (x == (seq_t)1)        /* reached the end of the trajectory */
            break;

        /* next term of the modified Collatz sequence */
        if ((x % 2) == 0)
            x = x / 2;
        else
            x = ((seq_t)3 * x + 1) / 2;

        ++pos;
    }

    return count;
}
