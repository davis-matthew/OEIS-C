/*
 * A343531  a(n) = Sum_{i=1..n} (prime(i+1)-prime(i))*prime(n+1-i).
 * https://oeis.org/A343531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (prime(i+1)-prime(i))*prime(n+1-i)
 * Range:     u8 n <= 9, u16 n <= 79, u32 n <= 9424, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper to fetch the k-th prime number. */
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* difference between consecutive primes */
        seq_t diff = (seq_t)get_prime(i + 1) - (seq_t)get_prime(i);
        /* prime at the symmetric position */
        seq_t p = (seq_t)get_prime(n + 1 - i);
        sum += diff * p;
    }
    return sum;
}
