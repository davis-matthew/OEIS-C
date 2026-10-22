/*
 * A307434  a(n) is the smallest k such that the p-rank of (Z/kZ)* is 2, where
 *          p = prime(n) and (Z/kZ)* is the multiplicative group of integers
 *          modulo n.
 * https://oeis.org/A307434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307434: smallest k such that the p-rank of (Z/kZ)* is 2
   where p = prime(n).  For odd p, k is the smaller of
   (q1 * q2) and (p^2 * q1), where q1 < q2 are the two smallest
   primes ≡ 1 (mod p).  Special case: p = 2 → k = 8. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* Special case for p = 2 */
    if (p == 2)
        return (seq_t)8;

    seq_t q1 = 0, q2 = 0;
    seq_t k = (seq_t)(p + 1);   /* first candidate ≥ p+1 */

    /* Find the smallest prime q1 ≡ 1 (mod p) */
    while (1) {
        if (is_prime((idx_t)k)) {
            q1 = k;
            break;
        }
        k += p;
    }

    /* Find the next prime q2 ≡ 1 (mod p) after q1 */
    k = q1 + p;
    while (1) {
        if (is_prime((idx_t)k)) {
            q2 = k;
            break;
        }
        k += p;
    }

    /* Compute the two possible candidates */
    seq_t prod1 = q1 * q2;       /* q1 * q2 */
    seq_t prod2 = p * p * q1;    /* p^2 * q1 */

    return (prod1 < prod2) ? prod1 : prod2;
}
