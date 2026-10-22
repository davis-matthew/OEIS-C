/*
 * A385198  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a prime power
 *          (A246655).
 * https://oeis.org/A385198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p^e || n} (p^e - 1) * Σ_{p^e || n} 1/(p^e - 1) =
 *            Σ_{p^e || n} ( ∏_{p^f || n} (p^f - 1) ) / (p^e - 1)
 * Range:     u8 n <= 414, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t tmp = n;
    seq_t uphi = 1;

    /* At most 5 distinct primes for n <= 10000, so 10 slots are safe. */
    seq_t t[10];
    int tcnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* Compute p^e in seq_t */
            seq_t powpe = 1;
            for (int i = 0; i < e; ++i)
                powpe *= (seq_t)p;
            seq_t ti = powpe - 1;
            t[tcnt++] = ti;
            uphi *= ti;
        }
    }

    if (tmp > 1) { /* remaining prime factor > sqrt(original n) */
        seq_t powpe = tmp;  /* tmp is prime, exponent 1 */
        seq_t ti = powpe - 1;
        t[tcnt++] = ti;
        uphi *= ti;
    }

    seq_t result = 0;
    for (int i = 0; i < tcnt; ++i)
        result += uphi / t[i];

    return result;
}
