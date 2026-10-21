/*
 * A244846  Least triangular number t>p such that q=p+t is also prime, where
 *          p=prime(n).
 * https://oeis.org/A244846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least triangular number t > p such that p + t is prime,
 *            where p = prime(n)
 * Range:     u8 n <= 18, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions from the common library.  They are available at run
 * time and do not need to be implemented here. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Obtain the n-th prime p. */
    seq_t p = (seq_t)get_prime(n);

    /* Find the smallest k such that the k-th triangular number
     *   t_k = k*(k+1)/2  exceeds p.  We start from k = 1 and
     *   incrementally build the triangular numbers to avoid big
     *   multiplications. */
    idx_t k = 1;          /* current index in the triangular sequence */
    seq_t t = 1;          /* t_1 = 1 */

    /* Increment k until we reach a triangular number greater than p. */
    while (t <= p) {
        ++k;
        t += (seq_t)k;    /* t_{k} = t_{k-1} + k */
    }

    /* Now t > p.  Search forward for the first t such that p + t is prime. */
    while (true) {
        seq_t q = p + t;
        if (isPrimeGeneric(q))
            return t;     /* found the least qualifying triangular number */

        /* Move to the next triangular number. */
        ++k;
        t += (seq_t)k;
    }
}
