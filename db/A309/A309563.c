/*
 * A309563  Number of cyclic permutations of length n that avoid the patterns
 *          123 and 231.
 * https://oeis.org/A309563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Φ((n+1)/2) if n is odd; * a(n) = Φ(n/2) if n ≡ 0 (mod 4);
 *            * a(n) = Φ((n+2)/4) + Φ(n/2) if n ≡ 2 (mod 4) and n > 2; * a(1)
 *            = a(2) = 1 * (Φ = Euler's totient function)
 * Range:     u8 n <= 385, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple totient calculation.  Works for n up to at least 32 000
 * which is sufficient for the ranges tested (n ≤ 10 000).
 */
static seq_t totient(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t m  = (uint64_t)n;
    uint64_t res = m;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            res -= res / p;
        }
    }
    if (m > 1)
        res -= res / m;
    return (seq_t)res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;

    /* Odd length. */
    if (n & 1ULL) {
        idx_t t = (n + 1) / 2;
        return totient(t);
    }

    /* Even length. */
    if ((n & 3ULL) == 0) { /* n ≡ 0 (mod 4) */
        idx_t t = n / 2;
        return totient(t);
    } else { /* n ≡ 2 (mod 4), n > 2 */
        idx_t t1 = (n + 2) / 4;
        idx_t t2 = n / 2;
        return totient(t1) + totient(t2);
    }
}
