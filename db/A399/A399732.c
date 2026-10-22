/*
 * A399732  The sum of the unitary divisors d of n such that n/d is cubefree.
 * https://oeis.org/A399732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = multiplicative function a(p^e) = p^e + 1 if e ≤ 2 p^e if
 *            e ≥ 3 which equals the sum of unitary divisors d of n with n/d
 *            cubefree
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* uint64_t */

/* Return the A399732 value for 1‑based index n (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Work on a 64‑bit copy for factorisation. */
    uint64_t rem = (uint64_t)n;
    seq_t res = (seq_t)1;

    /* Handle factor 2 separately. */
    int exp = 0;
    while ((rem & 1ULL) == 0) {
        ++exp;
        rem >>= 1;
    }
    if (exp > 0) {
        seq_t p_pow = (seq_t)1;
        for (int i = 0; i < exp; ++i) p_pow *= (seq_t)2;
        seq_t factor = p_pow + ((exp <= 2) ? (seq_t)1 : (seq_t)0);
        res *= factor;
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                ++e;
                rem /= p;
            }
            seq_t p_pow = (seq_t)1;
            for (int i = 0; i < e; ++i) p_pow *= (seq_t)p;
            seq_t factor = p_pow + ((e <= 2) ? (seq_t)1 : (seq_t)0);
            res *= factor;
        }
    }

    /* If a prime factor larger than sqrt(rem) remains. */
    if (rem > 1) {
        seq_t factor = (seq_t)rem + (seq_t)1;  /* exponent = 1  ⇒  e ≤ 2 */
        res *= factor;
    }

    return res;
}
