/*
 * A260928  Number of positive integers k < prime(n)/2 such that k + k' is a
 *          square, where k' is the unique integer among 1, ..., prime(n)-1
 *          such that k*k' == 1 (mod prime(n)).
 * https://oeis.org/A260928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 4712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260928: for the n‑th prime p = prime(n), count the
   number of positive integers k < p/2 such that k + k′ is a perfect
   square.  Here k′ is the unique integer 1 ≤ k′ ≤ p−1 with
   k·k′ ≡ 1 (mod p).  The function performs a direct scan of the
   admissible k's, computing the modular inverse by the extended
   Euclidean algorithm (no memory‑heavy modular exponentiation). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Compute the modular inverse of k modulo p (p prime),
   using the extended Euclidean algorithm.  All values are
   safely within 64‑bit signed integers for the primes used
   in the sequence. */
static inline seq_t modinv_uint64(idx_t k, idx_t p)
{
    int64_t a = (int64_t)p;
    int64_t b = (int64_t)k;
    int64_t x0 = 0, x1 = 1;

    while (b != 0) {
        int64_t q = a / b;
        int64_t tmp = a - q * b; a = b; b = tmp;
        tmp = x0 - q * x1;       x0 = x1; x1 = tmp;
    }
    /* a is gcd(p,k); for prime p and 1≤k<p, a must be 1. */
    if (x0 < 0) x0 += (int64_t)p;
    return (seq_t)x0;
}

/* Integer square root for 0 <= n <= 2^64-1. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = 1ULL << 32;           /* high > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime. */
    seq_t p64 = (seq_t)get_prime(n);
    if (p64 <= 2) return 0;

    idx_t p = (idx_t)p64;
    idx_t kmax = (p - 1) / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t inv = modinv_uint64(k, p);
        seq_t sum = (seq_t)k + inv;          /* k + k' < 2p <= 2^18 */
        idx_t s = (idx_t)sum;
        idx_t r = isqrt_u64(s);
        if ((r * r) == s)
            ++count;
    }

    return count;
}
