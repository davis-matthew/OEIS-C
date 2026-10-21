/*
 * A298862  Rank of n-th twice-prime when all the primes and twice-primes are
 *          jointly ranked.
 * https://oeis.org/A298862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = π(2·pₙ) + π(pₙ), where pₙ is the n‑th prime
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* n‑th prime (uint32_t is sufficient for all useful n) */
    uint32_t p = get_prime(n);

    /* prime count up to 2·p */
    idx_t twice = (idx_t)p * 2;
    seq_t pi_twice = compute_pi(twice);

    /* prime count up to p (stars whose double is ≤ 2·p) */
    seq_t pi_half = compute_pi((idx_t)p);

    /* rank of the twice‑prime 2·p in the combined list of primes and twice‑primes */
    return pi_twice + pi_half;
}
