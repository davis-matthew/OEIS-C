/*
 * A385045  The sum of the unitary divisors of n that are 5-rough numbers
 *          (A007310).
 * https://oeis.org/A385045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = ∏_{p^e‖n, p>3} (p^e + 1). For primes p≤3 the factor is 1
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t nn = (uint64_t)n;

    if (nn <= 1) return result;          /* a(1) = 1 */

    /* Remove factors 2 */
    if ((nn & 1) == 0) {
        while ((nn & 1) == 0) nn >>= 1;
    }

    /* Remove factors 3 */
    if (nn % 3 == 0) {
        while (nn % 3 == 0) nn /= 3;
    }

    /* Handle the remaining part using trial division */
    for (uint64_t p = 5; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            int e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            /* p > 3 by construction */
            seq_t pe = 1;
            seq_t base = (seq_t)p;
            for (int i = 0; i < e; ++i) pe = pe * base; /* p^e */
            seq_t term = pe + 1;                     /* (p^e + 1) */
            result = result * term;
        }
    }

    /* If a prime factor > 3 remains */
    if (nn > 1) {
        seq_t term = (seq_t)nn + 1;
        result = result * term;
    }

    return result;
}
