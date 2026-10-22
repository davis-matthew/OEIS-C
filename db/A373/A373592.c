/*
 * A373592  Number of primes congruent to 2 modulo 3 dividing n (with
 *          multiplicity).
 * https://oeis.org/A373592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors p of n with p ≡ 2 (mod 3), *
 *            counted with multiplicity. This implementation factors n by *
 *            trial division up to √n; for the target ranges (n ≤ 10 000) it *
 *            runs in negligible time
 * Range:     u8 n <= 9307, u16 n <= 9307, u32 n <= 9307, u64 n <= 9307,
 *            u128 n <= 9307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* a(1) = 0; for safety return 0 for n < 1 */
    if (n < 1) return 0;

    seq_t count = 0;
    uint64_t m = n;          /* use 64‑bit arithmetic for factorisation */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (p % 3ULL == 2ULL) {
                /* count multiplicity of this prime */
                do {
                    m /= p;
                    ++count;
                } while (m % p == 0);
            } else {
                /* skip over all powers of p that are not ≡ 2 (mod 3) */
                while (m % p == 0) m /= p;
            }
        }
    }

    /* Any remaining factor > 1 is prime. */
    if (m > 1 && m % 3ULL == 2ULL) {
        ++count;   /* exponent is 1 because m > √original n */
    }

    return count;
}
