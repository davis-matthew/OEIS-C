/*
 * A364391  a(n) = n - (largest nontrivial divisor of n, or 0 if there is
 *          none).
 * https://oeis.org/A364391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (largest nontrivial divisor of n, or 0 if none)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th term of OEIS A364391. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset: the sequence is defined for n >= 1.  The
       behavior for n < 1 is irrelevant, but we return a sensible
       value. */
    if (n < 2) {
        return (seq_t)n;   /* a(0)=0, a(1)=1 as per definition */
    }

    /* Find the smallest divisor d > 1 of n.  For a prime n no such
       divisor exists, so we return n itself.  The search stops at
       sqrt(n); using n/d avoids overflow on d*d. */
    idx_t d;
    for (d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            break;
        }
    }

    /* If no divisor was found, n is prime and the largest
       nontrivial divisor is 0; hence the result is n. */
    if (d > n / d) { /* loop finished without break: n is prime */
        return (seq_t)n;
    }

    /* Largest nontrivial divisor = n / d.  Compute using seq_t
       arithmetic for safety with arbitrary-precision builds. */
    seq_t nd = (seq_t)n;
    seq_t largest = nd / (seq_t)d;

    return nd - largest;
}
