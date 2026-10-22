/*
 * A352996  a(n) = n*(n+1)/2 mod (sum (with multiplicity) of prime factors of
 *          n).
 * https://oeis.org/A352996
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2) mod A001414(n) where A001414(n) is the sum of
 *            prime factors of n (with multiplicity)
 * Range:     u8 n <= 22, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is the index (idx_t); result is in seq_t. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the triangular number n*(n+1)/2 in seq_t. */
    seq_t tn = (seq_t)n;
    seq_t triangle = tn * (tn + 1) / 2;

    /* Sum the prime factors of n (with multiplicity). */
    uint64_t temp = (uint64_t)n;
    seq_t sum = 0;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        while (temp % p == 0) {
            sum += (seq_t)p;   /* cast to seq_t for accumulation */
            temp /= p;
        }
    }
    if (temp > 1) {
        sum += (seq_t)temp;     /* remaining prime factor */
    }

    /* Return the triangular number modulo the sum of prime factors. */
    return triangle % sum;
}
