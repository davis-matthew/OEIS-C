/*
 * A319524  a(n) is the smallest number that belongs simultaneously to the two
 *          arithmetic progressions prime(n) + m*prime(n+1) and prime(n+1) +
 *          m*prime(n+2), m >= 1, n >= 1.
 * https://oeis.org/A319524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest number in the intersection of * the arithmetic
 *            progressions prime(n)+m*prime(n+1) and * prime(n+1)+m*prime(n+2)
 *            for m ≥ 1
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 6541, u64 n <= 9528,
 *            u128 n <= 9528, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Include helper functions – assumed to be present in the same
   framework as provided by the build system. */
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n < 1, return 0 */

    /* Obtain the required primes.  get_prime is 1-indexed. */
    seq_t p_n  = get_prime(n);
    seq_t p_n1 = get_prime(n + 1);
    seq_t p_n2 = get_prime(n + 2);

    /* Base values for the two progressions */
    seq_t base1 = p_n + p_n1;
    seq_t base2 = p_n1 + p_n2;
    seq_t need = (base1 > base2) ? base1 : base2;

    /* CRT setup for coprime moduli: m = p_n1, n = p_n2 */
    seq_t a1 = p_n;          /* k ≡ a1 (mod m) */
    seq_t m  = p_n1;
    seq_t b1 = p_n1;        /* k ≡ b1 (mod n) */
    seq_t n_mod = p_n2;

    /* Compute difference modulo n */
    seq_t diff = (a1 > b1) ? (b1 + n_mod - a1) : (b1 - a1);

    /* Modular inverse of m modulo n using Fermat's little theorem */
    seq_t inv = mod_pow(m, (uint64_t)(n_mod - 2), n_mod);

    /* CRT solution in [0, m*n_mod) */
    seq_t mult = (diff * inv) % n_mod;
    seq_t x0   = a1 + m * mult;

    seq_t M = m * n_mod;
    if (x0 >= M) x0 -= M;  /* bring into canonical range */

    /* Ensure that the solution lies in the valid range
       where both progressions use m ≥ 1. */
    seq_t result = x0;
    if (result < need) {
        seq_t t = (need - result + M - 1) / M; /* ceil((need-result)/M) */
        result += t * M;
    }

    return result;
}
