/*
 * A332426  Number of unordered pairs of self-avoiding paths with nodes that
 *          cover all vertices of a convex n-gon.
 * https://oeis.org/A332426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*2^(n-6)*(2^(n-3)-1) for n > 2 a(1) = a(2) = 0 The
 *            implementation handles small n with a division by a power of two
 *            to avoid negative exponents
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the n-th term of OEIS sequence A332426 */
seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 2)
        return 0;

    seq_t result;

    if (n <= 5) {
        /* For n <= 5, 2^(n-6) would be fractional; cancel the
           fractional part by dividing before multiplying. */
        seq_t t = (seq_t)n * (seq_t)(n - 1);
        seq_t divisor = pow2(6 - n);   /* 6-n is positive */
        t = t / divisor;               /* exact integer division */
        seq_t inner = pow2(n - 3) - 1;  /* n-3 >= 0 for n>=3 */
        result = t * inner;
    } else {
        /* For n >= 6, all exponents are non‑negative */
        seq_t base = (seq_t)n * (seq_t)(n - 1);
        seq_t p1   = pow2(n - 6);
        seq_t p2   = pow2(n - 3) - 1;
        result = base * p1 * p2;
    }

    return result;
}
