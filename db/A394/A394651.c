/*
 * A394651  a(n) = tau(n) + Sum_{p|n, p prime, gcd(p,n/p) = 1} (p-1).
 * https://oeis.org/A394651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) + ∑_{p|n, p prime, gcd(p,n/p)=1} (p-1) where
 *            tau(n) is the number of positive divisors of n. The sum runs
 *            over unitary prime divisors of n – i.e. primes dividing n to the
 *            first power
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; a(0) is undefined, return 0 */

    uint64_t N = (uint64_t)n;      /* work in 64‑bit for factoring */
    seq_t tau = 1;                 /* divisor count */
    seq_t sum = 0;                 /* contribution of unitary prime divisors */

    /* factor 2 separately */
    if ((N & 1) == 0) {
        size_t exp = 0;
        do {
            N >>= 1;
            exp++;
        } while ((N & 1) == 0);
        tau *= (seq_t)(exp + 1);
        if (exp == 1) sum += (seq_t)(1);   /* 2‑1 = 1 */
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= N / p; p += 2) {
        if (N % p == 0) {
            size_t exp = 0;
            do {
                N /= p;
                exp++;
            } while (N % p == 0);
            tau *= (seq_t)(exp + 1);
            if (exp == 1) sum += (seq_t)(p - 1);
        }
    }

    /* if N > 1, it is a remaining prime factor (exponent 1) */
    if (N > 1) {
        tau *= (seq_t)2;           /* multiply by (1+1) */
        sum += (seq_t)(N - 1);
    }

    return tau + sum;
}
