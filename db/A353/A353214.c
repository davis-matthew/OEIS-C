/*
 * A353214  a(n) = 2^A007013(4) mod prime(n); the last term of this sequences
 *          is when a(n) = 1.
 * https://oeis.org/A353214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini
 * Formula:   a(n) = 2^(2^127 − 1) mod prime(n), expressed in the centered
 *            residue system {−⌊p/2⌋, …, ⌊p/2⌋}
 * Range:     i8 n <= 31, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

/* compute a(n) for n ≥ 1, using 1‑based indexing as in OEIS */
seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (prime(1) = 2). */
    seq_t p = (seq_t)get_prime(n);

    /* Special case: prime 2 is the only even prime, 2^k mod 2 = 0 for k>0 */
    if (p == 2)
        return (seq_t)0;            /* 0 is already the centered residue */

    /* φ(p) = p−1 (Euler's totient for a prime) */
    seq_t phi = p - 1;

    /* Compute 2^127 mod φ(p). 127 fits in uint64_t. */
    seq_t pow2_127 = mod_pow((seq_t)2, (uint64_t)127, phi);

    /* (2^127−1) mod φ(p) = (pow2_127 − 1) mod φ(p) */
    seq_t exp_mod = (pow2_127 + phi - 1) % phi;

    /* Compute 2^(2^127−1) mod prime(n) using the reduced exponent. */
    seq_t r = mod_pow((seq_t)2, (uint64_t)exp_mod, p);

    /* Convert to centered residue in range −⌊p/2⌋ … ⌊p/2⌋. */
    seq_t half = p / 2;               /* floor(p/2) */
    if (r > half)
        r -= p;

    return r;
}
