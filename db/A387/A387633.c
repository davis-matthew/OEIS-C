/*
 * A387633  a(n) = tau(n) + (n-1) * tau(rad(n)).
 * https://oeis.org/A387633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) + (n-1) * tau(rad(n))
 * Range:     u8 n <= 41, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes via prime factorisation: tau(n) = Π (e_i+1),  tau(rad(n)) = 2^{ω(n)} */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 for the OEIS offset; n == 0 is not used */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t tmp = n;
    seq_t tau = (seq_t)1;       /* product of (exponent+1) */
    idx_t distinct = 0;         /* number of distinct prime factors */

    /* factor 2 first */
    if ((tmp & 1ULL) == 0) {
        distinct++;
        idx_t exp = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            exp++;
        }
        tau *= (seq_t)(exp + 1);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            distinct++;
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            tau *= (seq_t)(exp + 1);
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (tmp > 1) {
        distinct++;
        tau *= (seq_t)2;        /* exponent 1 → (1+1) = 2 */
    }

    /* tau(rad(n)) = 2^{distinct} */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < distinct; ++i) {
        pow2 *= (seq_t)2;
    }

    /* final result */
    return tau + ((seq_t)n - 1) * pow2;
}
