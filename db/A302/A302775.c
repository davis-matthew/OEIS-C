/*
 * A302775  a(n) is the first term in A303762 that has prime(n) as one of its
 *          prime factors.
 * https://oeis.org/A302775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) is the product of the primes whose indices belong to
 *   S(n) = { , if n is odd:   {even indices ≤ n} ∪ {n}
 *           if n is even:  {1} ∪ {even indices ≤ n} ∪ {n–1} .
 *
 *   In other words,
 *     if n is even:  a(n) = 2 * ∏_{i=2,4,…,n} prime(i)  * prime(n-1)   (n>2)
 *                    a(2) = 2 * prime(2) = 6
 *     if n is odd :  a(n) =   ∏_{i=2,4,…,n} prime(i)  * prime(n)
 *
 *   The sequence a(n) is exactly A302775; it agrees with A303762(n).
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0, but return 0 safely */
        return 0;

    seq_t prod = 1;

    /* even n: include the prime 2 (index 1) once */
    if ((n & 1) == 0)
        prod = seq_mul(prod, (seq_t)get_prime(1));

    /* multiply all primes at even indices i (2 ≤ i ≤ n) */
    for (idx_t i = 2; i <= n; ++i)
        if ((i & 1) == 0)
            prod = seq_mul(prod, (seq_t)get_prime(i));

    /* finish with the odd index needed for this n */
    if ((n & 1) == 0) {
        if (n > 2)                   /* avoid duplicating prime 2 when n==2 */
            prod = seq_mul(prod, (seq_t)get_prime(n - 1));
    } else {
        prod = seq_mul(prod, (seq_t)get_prime(n));
    }

    return prod;
}
