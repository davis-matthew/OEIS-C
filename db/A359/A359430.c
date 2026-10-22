/*
 * A359430  a(n) = 1 if the arithmetic derivative of n is a multiple of 3,
 *          otherwise 0.
 * https://oeis.org/A359430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is a multiple of 3,
 *            else 0
 * Range:     u8 n <= 9890, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the prime factorization formula n' = n * Σ_i (e_i / p_i) */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first two trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    uint64_t N = (uint64_t)n;        /* value of n as 64‑bit integer */
    uint64_t t = N;                  /* working copy for factorization */
    uint64_t mod3 = 0;               /* running sum modulo 3 */

    /* Trial division to factor n */
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            /* term = exp * (N/p)  (mod 3)  */
            uint64_t term = (exp * ((N / p) % 3)) % 3;
            mod3 = (mod3 + term) % 3;
        }
    }

    /* If any prime factor > sqrt remains */
    if (t > 1) {
        /* Here exp = 1, p = t, term = (N/t) (mod 3) */
        uint64_t term = (N / t) % 3;
        mod3 = (mod3 + term) % 3;
    }

    return (seq_t)(mod3 == 0 ? 1 : 0);
}
