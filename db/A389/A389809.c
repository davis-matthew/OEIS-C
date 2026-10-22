/*
 * A389809  Sequence containing k-1 occurrences of prime(k) ordered by k.
 * https://oeis.org/A389809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(k) where k is the smallest integer s.t. k(k‑1)/2 ≥
 *            n * This implements the OEIS sequence A389809: the k‑1
 *            occurrences of * prime(k) ordered by k. The function works for
 *            any n ≥ 1 and is * independent of call order. It uses
 *            get_prime(k) provided by the * utilities to obtain the k‑th p...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* The sequence starts at n = 1.  For n = 0 the behaviour is
     * undefined and the function will return 0. */
    if (n == 0) return (seq_t)0;

    /* Find smallest k with k(k-1)/2 ≥ n.  Since n ≤ 10^4 in the
     * requirements, a linear search is inexpensive. */
    idx_t k = 2;
    while (k * (k - 1) / 2 < n) {
        k++;
    }

    /* Return the k‑th prime.  The get_prime function handles
     * caching and is already efficient. */
    return (seq_t)get_prime(k);
}
