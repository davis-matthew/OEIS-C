/*
 * A259232  Smallest prime q > p such that q + p is a square, where p is the
 *          n-th prime.
 * https://oeis.org/A259232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 6071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A259232.
   For the n‑th term, let p be the n‑th prime.  We look for the
   smallest prime q > p satisfying that p + q is a perfect square.
   If k² = p + q, then q = k² – p and we must have k² > 2p.
   Starting from k = ⌊√(2p)⌋ + 1, we increase k until k² – p is prime.
   Complexity per call is O(√p).  The function is thread‑safe and
   does not rely on any mutable global state. */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; ignore n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime p. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute 2p and its integer square root. */
    seq_t two_p = p + p;
    seq_t sqrt_two_p = floor_sqrt(two_p);

    /* Start testing from the first integer greater than sqrt(2p). */
    idx_t k = (idx_t)sqrt_two_p + 1;

    /* Increment k until k² – p is a prime. */
    for (;; ++k)
    {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t q    = k_sq - p;
        if (is_prime_big(q))
            return q;
    }
}
