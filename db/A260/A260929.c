/*
 * A260929  Least positive integer k < prime(n) such that k + k' is a
 *          triangular number, where k' is the unique integer among
 *          1,...,prime(n)-1 with k*k' == 1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A260929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A260929
   a(n) = the least positive integer k < prime(n) such that
          k + k' is a triangular number, where k' ≡ k⁻¹ (mod prime(n)).
   If no such k exists, a(n) = 0.
   Implementation uses the provided utilities: get_prime, mod_pow, floor_sqrt.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k);          */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/floor_sqrt.h"  /* seq_t floor_sqrt(seq_t x);            */

seq_t a(idx_t n) {
    /* Ensure the index is valid.  a(1) corresponds to prime(1) = 2. */
    if (n == 0) return 0;

    /* Obtain the n-th prime.  It fits in 32 bits for all reasonable n. */
    uint32_t p32 = get_prime(n);
    seq_t   prime = (seq_t)p32;

    /* Search for the smallest k < prime satisfying the condition. */
    for (uint64_t k = 1; k < (uint64_t)p32; ++k) {
        seq_t k_ts   = (seq_t)k;

        /* k' is the modular inverse of k modulo prime.           */
        seq_t inv = mod_pow(k_ts, (uint64_t)p32 - 2, prime);

        seq_t sum = k_ts + inv;                   /* k + k'   */
        seq_t D   = (seq_t)1 + (seq_t)8 * sum;    /* 1 + 8*sum */

        seq_t s = floor_sqrt(D);
        if (s * s == D && (s - 1) % (seq_t)2 == 0) {
            /* k + k' is triangular.  Return the smallest such k. */
            return k_ts;
        }
    }

    /* No suitable k found. */
    return (seq_t)0;
}
