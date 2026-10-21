/*
 * A262463  If n is prime a(n) = n else a(n) = nextprime(reverse(n)), where
 *          "next prime" is the smallest prime >= n, see A007918.
 * https://oeis.org/A262463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n is prime; else let b = reverse(n); if b is prime
 *            a(n)=b else a(n)=next prime ≥ b
 * Range:     u8 n <= 103, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   is_prime, is_prime_big, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/reverse.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Prime test for the 64‑bit index (fast). */
    if (is_prime(n))
        return (seq_t)n;

    /* Decimal reverse of n. */
    seq_t rev = reverse((seq_t)n);

    /* If the reverse itself is prime, return it. */
    if (is_prime_big(rev))
        return rev;

    /* Otherwise, find the next prime ≥ rev by simple trial. */
    seq_t cand = rev + 1;
    while (!is_prime_big(cand))
        cand += 1;
    return cand;
}
