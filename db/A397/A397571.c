/*
 * A397571  Field trace Tr_{Q(zeta_p)/Q}(T_p(zeta_p)) of the p-term truncation
 *          of Ramanujan's third-order mock theta function f(q) at a primitive
 *          p-th root of unity, where p = prime(n+1).
 * https://oeis.org/A397571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3, n=1; for n≥2, let p = prime(n+1). If p ≡ 1 (mod 6)
 *            then a(n) = 2*p − 1, else (p ≡ 5 (mod 6)) a(n) = p − 1
 * Range:     u8 n <= 32, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)3;
    }
    /* Obtain the (n+1)-th prime: get_prime(k) returns the k-th prime. */
    seq_t p = (seq_t)get_prime(n + 1);    /* p ≥ 5 and odd */

    /* If p ≡ 1 (mod 6), use 2*p − 1; otherwise use p − 1. */
    if ((p % 6u) == 1u) {
        return (seq_t)2 * p - (seq_t)1;
    } else {
        return p - (seq_t)1;
    }
}
