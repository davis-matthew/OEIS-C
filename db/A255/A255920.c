/*
 * A255920  Number of primes p with p < n such that n^(p-1) == 1 (mod p^2)
 *          i.e., number of Wieferich primes to base n less than n.
 * https://oeis.org/A255920
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of primes p < n such that n^(p-1) ≡ 1 (mod p²) */
/* Uses a sieve to generate primes below n and modular exponentiation to test each. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    size_t primes_cnt = 0;
    uint64_t *primes = NULL;
    /* Build list of primes up to n-1 (inclusive). */
    build_primes_upto(n - 1, &primes, &primes_cnt);

    seq_t result = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p >= n) break;                 /* should not happen, but keep safe */

        uint64_t mod_val = p * p;           /* p² fits in 64‑bit (p ≤ n ≤ 10000 here) */
        seq_t mod = (seq_t)mod_val;
        seq_t base = (seq_t)(n % mod_val);  /* reduce n modulo p² to avoid overflow */

        seq_t pow_res = mod_pow(base, p - 1, mod);
        if (pow_res == 1)
            ++result;
    }

    free(primes);   /* the array produced by build_primes_upto is heap‑allocated */
    return result;
}
