/*
 * A258825  a(n) = Number of times the k-th term is equal to k in the modified
 *          Collatz trajectory of n, when counting the initial term n as the
 *          term zero: n, A014682(n), A014682(A014682(n)), ...
 * https://oeis.org/A258825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k such that the k‑th modified Collatz iterate
 *            of n equals k. Here the modified Collatz function is: f(x) = x/2
 *            if x is even, (3x+1)/2 if x is odd. The initial term n is
 *            counted as term zero
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* Undefined for n < 1; return 0 for safety. */
        return 0;

    seq_t cur   = n;     /* Current term in the trajectory. */
    seq_t count = 0;     /* Number of times cur == k. */
    idx_t  k    = 0;     /* Current iteration index. */

    while (1) {
        if (cur == (seq_t)k)           /* cur equals the iteration index? */
            count++;

        if (cur == (seq_t)1)           /* once we hit 1, future terms are 1 or 2,
                                          so no further equalities possible. */
            break;

        /* Apply the modified Collatz step. */
        if ((cur & 1) == 0)            /* even */
            cur = cur >> 1;
        else                           /* odd */
            cur = (cur * 3 + 1) >> 1;  /* (3*cur+1)/2 */

        k++;
    }

    return count;
}
