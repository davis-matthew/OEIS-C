/*
 * A360164  a(n) is the sum of the square roots of the unitary divisors of n
 *          that are odd squares.
 * https://oeis.org/A360164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, gcd(d, n/d)=1, d odd square} sqrt(d).
 *            Multiplicative rule: a(2^e) = 1 a(p^e) = 1 if e is odd a(p^e) =
 *            p^(e/2) + 1 if e is even (p odd prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Computes a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not used in the OEIS range */

    seq_t result = 1;              /* multiplicative identity        */
    idx_t m = n;

    /* Remove all factors of 2 – they never contribute change. */
    while ((m & 1ULL) == 0) {
        m >>= 1;
    }

    /* Factor the remaining odd part of n. */
    idx_t temp = m;
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if ((e & 1) == 0) {  /* even exponent */
                seq_t t   = seq_pow((seq_t)p, e / 2); /* p^(e/2)   */
                t += 1;              /* +1 for the odd‑square divisor 1 */
                result *= t;
            }
            /* when e is odd the contribution is 1, no change to result */
        }
    }

    /* If a prime factor larger than sqrt(n) remains, its exponent is 1 (odd),
       so its contribution is 1 – nothing to do. */
    return result;
}
