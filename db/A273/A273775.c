/*
 * A273775  Smallest prime p > n + 1 where a base b exists with abs(b - p) = n
 *          such that b^(p-1) == 1 (mod p^2).
 * https://oeis.org/A273775
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 20, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A273775:  smallest prime p > n+1 such that there exists a base b
   with |b-p| = n and b^(p-1) ≡ 1 (mod p²).  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4.  Below that the value
       is irrelevant to the tests. */
    if (n < 4) return 0;

    /* Start searching from the first integer greater than n+1. */
    idx_t p = n + 2;
    /* Make the initial candidate odd (except if it is 2, which never
       occurs here because n ≥ 4). */
    if ((p & 1ULL) == 0) ++p;

    for (;; p += 2) {            /* iterate over odd candidates only */
        if (!is_prime(p)) continue;

        /* Compute modulus p².  For seq_t of 64 bits the product is
           guaranteed to fit, because for n ≤ 10 000 the searched prime
           values are far below 4 294 967 296.  The cast to seq_t is
           therefore safe. */
        uint64_t mod64 = (uint64_t)p * (uint64_t)p;
        seq_t mod = (seq_t)mod64;

        seq_t base1 = (seq_t)(p + n);   /* b = p + n  */
        seq_t base2 = (seq_t)(p - n);   /* b = p − n  */

        /* Test the two possible bases.  The exponents are p‑1. */
        if (mod_pow(base1, p - 1, mod) == 1) return (seq_t)p;
        if (mod_pow(base2, p - 1, mod) == 1) return (seq_t)p;
    }
}
