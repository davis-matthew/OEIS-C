/*
 * A365767  a(n) is the number of primes between (prime(n))^3 and
 *          (prime(n+1))^3.
 * https://oeis.org/A365767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(prime(n+1)^3 – 1) – π(prime(n)^3)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th and (n+1)-th primes */
    idx_t p1 = get_prime(n);
    idx_t p2 = get_prime(n + 1);

    /* Compute their cubes using seq_t to avoid overflow */
    seq_t low  = (seq_t)p1 * p1 * p1;
    seq_t high = (seq_t)p2 * p2 * p2;

    /* Count primes strictly between low and high */
    seq_t count = compute_pi((idx_t)(high - 1)) - compute_pi((idx_t)low);
    return count;
}
