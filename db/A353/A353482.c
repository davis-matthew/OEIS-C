/*
 * A353482  a(n) = 1 if n is a squarefree semiprime of the form 4k+1,
 *          otherwise 0.
 * https://oeis.org/A353482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9925, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n is a squarefree semiprime ≡ 1 (mod 4); otherwise 0 */

#include <stdint.h>
#include <stdbool.h>

static bool _is_prime_u64(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d <= m / d; d += 2)
        if (m % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* n is of type idx_t (uint64_t). */
    uint64_t x = (uint64_t)n;

    /* n must be congruent to 1 (mod 4). */
    if ((x & 3u) != 1u) return (seq_t)0;

    /* Find the smallest prime factor p of x. */
    uint64_t p = 0;
    if (x % 2u == 0) {
        p = 2u;
    } else {
        for (uint64_t d = 3u; d <= x / d; d += 2u) {
            if (x % d == 0) {
                p = d;
                break;
            }
        }
    }
    if (p == 0) return (seq_t)0;            /* x is prime */

    uint64_t q = x / p;

    /* x must be square‑free: check that p² does not divide x. */
    if (q % p == 0) return (seq_t)0;        /* repeated factor */

    /* q must be prime (x must have exactly two distinct prime factors). */
    if (!_is_prime_u64(q)) return (seq_t)0;

    /* All conditions satisfied. */
    return (seq_t)1;
}
