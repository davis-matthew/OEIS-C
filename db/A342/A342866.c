/*
 * A342866  The number of elements in the continued fraction for phi(n)/n,
 *          where phi is the Euler totient function (A000010).
 * https://oeis.org/A342866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of the continued fraction of φ(n)/n (Euler's
 *            totient φ(n) divided by n). The continued fraction expansion is
 *            obtained with the Euclidean algorithm, and its number of terms
 *            is counted
 * Range:     u8 n <= 515, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* If n==0 we return 0, although the sequence is defined
       for n>=1. */
    if (n == 0) return 0;

    /* Compute φ(n) using trial division.  All arithmetic is
       performed on 64‑bit unsigned integers; the result is
       then cast to seq_t.  This works because φ(n) <= n. */
    uint64_t num = (uint64_t)n;
    uint64_t phi = num;
    uint64_t tmp = num;

    /* Trial division up to sqrt(tmp).  Avoid overflow in
       d*d by using tmp/d. */
    for (uint64_t d = 2; d <= tmp / d; ++d) {
        if (tmp % d == 0) {
            /* d is a prime factor of n.  Remove all
               copies of d from tmp.  Then update phi. */
            while (tmp % d == 0) tmp /= d;
            phi -= phi / d;
        }
    }
    /* If a prime factor > sqrt(n) remains, it appears only
       once. */
    if (tmp > 1) {
        phi -= phi / tmp;
    }

    /* Now compute the length of the continued fraction
       expansion of phi/n using the classical Euclidean
       algorithm.  The number of iterations equals the
       number of partial quotients, including the leading
       zero for n>1. */
    seq_t p = (seq_t)phi;
    seq_t q = (seq_t)n;
    seq_t len = 0;
    while (q != 0) {
        seq_t k = p / q;   /* quotient */
        (void)k;           /* quotient is not needed, only for
                               following Euclidean step */
        seq_t r = p % q;   /* remainder */
        p = q;
        q = r;
        ++len;
    }
    return len;
}
