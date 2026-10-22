/*
 * A395491  a(n) = tau(n) + (n-1) * Sum_{p|n, p prime} [gcd(p,n/p) = 1], where
 *          [ ] is the Iverson bracket.
 * https://oeis.org/A395491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = τ(n) + (n-1) * Σ_{p∣n, p prime} [gcd(p,n/p)=1], * where [
 *            ] is the Iverson bracket. * This is equivalent to a(n) = τ(n) +
 *            (n-1) * (number of unitary prime divisors of n)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, but return 0 as a safe fallback */
    uint64_t m = (uint64_t)n;      /* work in 64‑bit for factorisation */
    seq_t tau = 1;                 /* τ(n) */
    seq_t unitary = 0;             /* count of prime factors with exponent 1 */

    /* factor 2 */
    int exp = 0;
    while (m % 2 == 0) { m /= 2; exp++; }
    if (exp) {
        tau *= (seq_t)(exp + 1);
        if (exp == 1) unitary++;
    }

    /* odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) { m /= p; exp++; }
        if (exp) {
            tau *= (seq_t)(exp + 1);
            if (exp == 1) unitary++;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        tau *= (seq_t)2;          /* exponent 1 */
        unitary++;
    }

    /* compute a(n) */
    seq_t result = tau + (seq_t)(n - 1) * unitary;
    return result;
}
