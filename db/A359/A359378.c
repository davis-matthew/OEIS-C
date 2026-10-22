/*
 * A359378  Dirichlet inverse of A359377, where A359377(n) = 1 if 3*n is
 *          squarefree, otherwise 0.
 * https://oeis.org/A359378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9506, i16 n <= 9506, i32 n <= 9506, i64 n <= 9506,
 *            i128 n <= 9506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Dirichlet inverse of A359377
   Formula: a(1)=1; for n>1 a(n)
   equals (-1)^{Ω(n)} if 3∤n, otherwise 0
   Here Ω(n) is the total number of prime factors of n counting multiplicity.
   The function is multiplicative and for all n with a factor 3 the value is 0.
   Computed by simple trial division – O(√n) per call. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The index is 1‑based; the function is never called with n = 0. */
    if (n == 0) return (seq_t)0;          /* safety guard */

    /* Any multiple of 3 has value 0. */
    if (n % 3 == 0) return (seq_t)0;

    /* Count parity of prime factors with multiplicity (Ω(n)). */
    uint64_t m = (uint64_t)n;
    int parity = 0;                       /* 0 => even, 1 => odd */

    /* factor 2 */
    while ((m & 1ULL) == 0) {
        parity ^= 1;
        m >>= 1;
    }

    /* odd factors up to sqrt(m) */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        while (m % i == 0) {
            parity ^= 1;
            m /= i;
        }
    }

    /* if a prime factor > sqrt remains */
    if (m > 1) {
        parity ^= 1;
    }

    return (seq_t)(parity == 0 ? 1 : -1);
}
