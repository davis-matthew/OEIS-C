/*
 * A378879  a(n) = number of non-Pythagorean primes in the prime factorization
 *          of n (including multiplicities).
 * https://oeis.org/A378879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non‑Pythagorean primes (2 or primes ≡ 3 mod 4)
 *            in the prime factorization of n, counting multiplicities
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses simple trial division; n ≈ 10 000 in typical calls, so the cost is negligible */

#include <stdint.h>

/* External types provided by the build system */
typedef uint64_t idx_t;    /* index type */

/* The function signature as requested */
seq_t a(idx_t n)
{
    /* For the offset a(1) = 0; any n < 2 has no prime factors */
    if (n < 2)
        return 0;

    seq_t count = 0;                /* result to return */
    uint64_t m = (uint64_t)n;       /* work copy for factorisation */

    /* Handle factor 2 separately (non‑Pythagorean) */
    while ((m & 1ULL) == 0) {       /* m % 2 == 0 */
        ++count;
        m >>= 1;                    /* divide by 2 */
    }

    /* Trial division for odd factors up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        /* Fast skip: a prime that is 1 mod 4 contributes 0 to count */
        if ((p & 3) == 1) {
            while (m % p == 0) {
                m /= p;             /* discard factor */
            }
            continue;
        }
        /* p ≡ 3 mod 4 or p == 3: count all multiplicities */
        while (m % p == 0) {
            ++count;
            m /= p;
        }
    }

    /* If remaining m > 1, it is a prime > sqrt(original n) */
    if (m > 1) {
        /* Final prime factor: check if it is 2 or ≡ 3 mod 4 */
        if (m == 2 || (m & 3) == 3)
            ++count;
    }

    return count;
}
